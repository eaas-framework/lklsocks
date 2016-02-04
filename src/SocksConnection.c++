#include "SocksConnection.h"

#include <iostream>
#include <boost/asio.hpp>

#include "src/asio/stream_socket_service.hpp"



SocksConnection::SocksConnection(boost::asio::io_service &io_service) :
        hostSocket(io_service), remoteSocket(io_service) {
}

SocksConnection::~SocksConnection()
{
}

void SocksConnection::start() {
    std::cout << "started a new connection" << std::endl;

    std::cout << "connecting remote lkl socket" << std::endl;

    lkl::asio::ip::tcp::resolver resolver(this->remoteSocket.get_io_service());
    lkl::asio::ip::tcp::resolver::query query(lkl::asio::ip::tcp::v4(), "10.10.10.10", "5678");

    std::cout << "Connecting to 10.10.10.10 now" << std::endl;
    boost::asio::connect(this->remoteSocket, resolver.resolve(query));
    char request[] = "miep\n";
    size_t request_length = std::strlen(request);
    boost::asio::write(this->remoteSocket, boost::asio::buffer(request, request_length));



    std::cout << "now terminating it" << std::endl;
    this->hostSocket.close();
}
