#include "SocksConnection.h"

#include <iostream>
#include <functional>
#include <thread>
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <arpa/inet.h>

#include "src/asio/stream_socket_service.hpp"

namespace asio = boost::asio;

enum class SocksRequestVersion : uint8_t {
    REQUEST_SOCKS4 = 4
};

enum class SocksReplyVersion : uint8_t {
    REPLY_SOCKS4 = 0
};

enum class Socks4Command : uint8_t {
    CONNECT = 1,
    BIND = 2
};

enum class Socks4Reply : uint8_t {
    SUCCESS = 90,
    FAILED = 91,
    IDENTD_FAILED = 92,
    IDENTD_INVALID = 93
};

struct Socks4Request {
    SocksRequestVersion version;
    Socks4Command command;
    uint16_t destinationPort;
    uint32_t destinationAddress;
};

struct Socks4Response {
    SocksReplyVersion version;
    Socks4Reply command;
    uint16_t destinationPort;
    uint32_t destinationAddress;
};


SocksConnection::SocksConnection(boost::asio::io_service &io_service) :
        hostSocket(io_service), remoteSocket(io_service) {
}

SocksConnection::~SocksConnection()
{
}

void SocksConnection::start() {
    // start host connection thread
    this->hostThread = std::move(std::thread(std::bind(&SocksConnection::handshake, this)));
}

void SocksConnection::handshake() {
    Socks4Request request;
    asio::streambuf userdata;
    asio::read(this->hostSocket, asio::buffer(&request, sizeof(request)));
    asio::read_until(this->hostSocket, userdata, "\0");
    request.destinationPort = ntohs(request.destinationPort);
    request.destinationAddress = ntohl(request.destinationAddress);

    lkl::asio::ip::tcp::socket::endpoint_type endpoint(
            asio::ip::address_v4(request.destinationAddress),
            request.destinationPort);

    std::cout << "Connecting to " << endpoint << std::endl;

    boost::system::error_code ec;
    this->remoteSocket.connect(endpoint, ec);
    if (ec) {
        this->remoteSocket.close();

        Socks4Response response { SocksReplyVersion::REPLY_SOCKS4,
                                  Socks4Reply::FAILED,
                                  htons(request.destinationPort),
                                  htonl(request.destinationAddress) };
        asio::write(this->hostSocket, asio::buffer(&response, sizeof(response)));
        std::cout << "Connection error to remote site, closing down." << std::endl;
        this->hostSocket.close();
        return;
    }

    Socks4Response response { SocksReplyVersion::REPLY_SOCKS4,
                              Socks4Reply::SUCCESS,
                              htons(request.destinationPort),
                              htonl(request.destinationAddress) };
    asio::write(this->hostSocket, asio::buffer(&response, sizeof(response)));
    std::cout << "Connection to remote site successful, start proxying." << std::endl;

    // start remote reading thread
    this->remoteThread = std::move(std::thread(std::bind(&SocksConnection::receiveRemote, this)));
    this->receiveHost();
}

void SocksConnection::receiveRemote() {
    std::array<char, 1500> buf;
    while (true) {
        // LKL currently does not support SMP and one pending syscall
        // will block all further syscalls, so don't to blocking reads!
        while (this->remoteSocket.available() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        size_t length = this->remoteSocket.read_some(asio::buffer(buf));
        asio::write(this->hostSocket, asio::buffer(buf, length));

    }
}

void SocksConnection::receiveHost() {
    std::array<char, 1500> buf;
    while (true) {
        size_t length = this->hostSocket.read_some(asio::buffer(buf));
        asio::write(this->remoteSocket, asio::buffer(buf, length));
    }
}
