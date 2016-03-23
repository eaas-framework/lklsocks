#include "SocksServer.h"

#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>

#include "SocksConnection.h"

namespace asio = boost::asio;

SocksServer::SocksServer(unsigned short port,
                         asio::io_service &io_service) :
        io_service(io_service),
        acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(),
                                                     port)) {
    this->startAccept();
}

SocksServer::~SocksServer() {
    try {
        this->acceptor.cancel();
        this->acceptor.close();
    } catch (std::exception &e) {
        std::cerr << "Error closing listening socket: " << e.what() << std::endl;
    }
}

void SocksServer::stopConnection(SocksConnection::ptr_t const conn) {
    // make sure this is only called from the "main" thread
    this->io_service.post([=]() {
        // this sleep is required to "guarantee" that all other pointer
        // references are out of scope
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        this->connections.remove(conn);
    });
}

void SocksServer::startAccept() {
    SocksConnection::ptr_t connection = SocksConnection::create(this->acceptor.get_io_service(), *this);

    this->acceptor.async_accept(
            connection->getHostSocket(),
            boost::bind(&SocksServer::handleAccept, this, connection,
                        asio::placeholders::error));
}

void SocksServer::handleAccept(SocksConnection::ptr_t connection, boost::system::error_code const &error) {
    if (!error) {
        this->io_service.post([=]() {
            this->connections.push_back(connection);
            connection->start();
        });
    }
    this->startAccept();
}
