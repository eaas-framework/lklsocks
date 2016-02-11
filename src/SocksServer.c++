#include "SocksServer.h"

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>

#include "SocksConnection.h"

namespace asio = boost::asio;

SocksServer::SocksServer(unsigned short port,
                         asio::io_service &io_service) :
        acceptor(io_service,
                 asio::ip::tcp::endpoint(asio::ip::tcp::v4(),
                                                port))
{
    this->startAccept();
}

SocksServer::~SocksServer()
{
}

void SocksServer::startAccept() {
    SocksConnection::ptr_t connection = SocksConnection::create(this->acceptor.get_io_service());

    this->acceptor.async_accept(
            connection->getHostSocket(),
            boost::bind(&SocksServer::handleAccept, this, connection,
                        asio::placeholders::error));
}

void SocksServer::handleAccept(SocksConnection::ptr_t connection, boost::system::error_code const &error) {
    if (!error) {
        this->connections.push_back(connection);
        connection->start();
    }
    this->startAccept();
}
