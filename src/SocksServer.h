#ifndef SOCKSSERVER_H_
#define SOCKSSERVER_H_

#include <list>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "SocksConnection.h"

class SocksServer {
public:
    SocksServer(unsigned short port, boost::asio::io_service &io_service);
    virtual ~SocksServer();

    void stopConnection(SocksConnection::ptr_t const conn);
protected:
    boost::asio::io_service &io_service;
    boost::asio::ip::tcp::acceptor acceptor;
    std::list<SocksConnection::ptr_t> connections;

    void startAccept();
    void handleAccept(SocksConnection::ptr_t connection, boost::system::error_code const &error);
};

#endif /* SOCKSSERVER_H_ */
