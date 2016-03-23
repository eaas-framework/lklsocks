#include "SocksConnection.h"

#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <arpa/inet.h>

#include "src/asio/stream_socket_service.hpp"
#include "SocksServer.h"
#include "lkl_helper.h"
#include "ScopeGuard.h"

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


SocksConnection::SocksConnection(boost::asio::io_service &io_service,
                                 SocksServer &server) :
        hostSocket(io_service), remoteSocket(io_service), server(server) {
}

SocksConnection::~SocksConnection() {
    try {
        {
            std::lock_guard<std::mutex> lock(this->mutex);

            // ignore all errors here, we cannot reasonably recover
            boost::system::error_code ec;

            if (this->remoteSocket.is_open()) {
                this->remoteSocket.shutdown(
                        asio::ip::tcp::socket::shutdown_both, ec);
                this->remoteSocket.close(ec);
            }
            if (this->hostSocket.is_open()) {
                this->hostSocket.shutdown(asio::ip::tcp::socket::shutdown_both,
                                          ec);
                this->hostSocket.close(ec);
            }
        }

        if (this->hostThread.joinable()) {
            this->hostThread.join();
        }
        if (this->remoteThread.joinable()) {
            this->remoteThread.join();
        }
    } catch (...) {
        std::cerr << "An exception occurred in ~SockConnection(). The world may come to an end soon." << std::endl;
    }
}

void SocksConnection::start() {
    // start host connection thread
    this->hostThread = std::move(std::thread(std::bind(&SocksConnection::handshake, this)));
}

void SocksConnection::handshake() {
    lkl_thread_start();
    auto threadGuard = scopeGuard([] {
        lkl_thread_stop();
    });

    try {
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

        try {
            this->remoteSocket.connect(endpoint);
        } catch (boost::system::system_error &e) {
            std::cerr << "Connection error: " << e.code() << std::endl;

            Socks4Response response { SocksReplyVersion::REPLY_SOCKS4,
                                      Socks4Reply::FAILED,
                                      htons(request.destinationPort),
                                      htonl(request.destinationAddress) };
            asio::write(this->hostSocket, asio::buffer(&response, sizeof(response)));

            std::cerr << "Connection error to remote site, closing down." << std::endl;
            throw;
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
    } catch (...) {
        this->server.stopConnection(this->shared_from_this());
    }
}

void SocksConnection::receiveRemote() {
    lkl_thread_start();
    auto threadGuard = scopeGuard([] {
        lkl_thread_stop();
    });

    try {
    std::array<char, 1500> buf;
        while (true) {
            size_t length = this->remoteSocket.read_some(asio::buffer(buf));
            asio::write(this->hostSocket, asio::buffer(buf, length));
        }
    } catch (boost::system::system_error &e) {
        if (e.code() == boost::asio::error::eof) {
            try {
                this->server.stopConnection(this->shared_from_this());
            } catch (std::bad_weak_ptr &e) {
                // ignore this exception: if we cannot call shared_from_this(),
                // the connection's d'tor is already running (or about to)
                // and there is no point in stopping the connection again.
                // shared_from_this() is still, however, the most convenient
                // way to pass "this" to stopConnection
            }
        }
    }

#if 0
        // LKL currently does not support SMP and one pending syscall
        // will block all further syscalls, so don't to blocking reads!
        while (this->remoteSocket.receive(
                asio::buffer(buf),
                asio::socket_base::message_peek | MSG_DONTWAIT, ec) == 0) {
            if (ec == boost::asio::error::eof || ec == boost::asio::error::bad_descriptor) {
                try {
                    this->server.stopConnection(this->shared_from_this());
                } catch (std::bad_weak_ptr &e) {
                    // ignore this exception: if we cannot call shared_from_this(),
                    // the connection's d'tor is already running (or about to)
                    // and there is no point in stopping the connection again.
                    // shared_from_this() is still, however, the most convenient
                    // way to pass "this" to stopConnection
                }
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        size_t length = this->remoteSocket.read_some(asio::buffer(buf));
        asio::write(this->hostSocket, asio::buffer(buf, length));
#endif
}

void SocksConnection::receiveHost() {
    try {
        std::array<char, 1500> buf;
        while (true) {
            size_t length = this->hostSocket.read_some(asio::buffer(buf));
            asio::write(this->remoteSocket, asio::buffer(buf, length));
        }
    } catch (boost::system::system_error &e) {
        if (e.code() == boost::asio::error::eof) {
            try {
                this->server.stopConnection(this->shared_from_this());
            } catch (std::bad_weak_ptr &e) {
                // ignore this exception: if we cannot call shared_from_this(),
                // the connection's d'tor is already running (or about to)
                // and there is no point in stopping the connection again.
                // shared_from_this() is still, however, the most convenient
                // way to pass "this" to stopConnection
            }
        }
    }
}
