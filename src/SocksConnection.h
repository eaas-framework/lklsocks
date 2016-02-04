#ifndef SOCKSCONNECTION_H_
#define SOCKSCONNECTION_H_

#include <boost/asio/ip/tcp.hpp>
#include <memory>

#include "src/asio/stream_socket_service.hpp"
#include "asio/ip/tcp.hpp"

class SocksConnection {
public:
    typedef std::shared_ptr<SocksConnection> ptr_t;

    static ptr_t create(boost::asio::io_service &io_service) {
        return ptr_t(new SocksConnection(io_service));
    }

    virtual ~SocksConnection();

    boost::asio::ip::tcp::socket &getHostSocket() {
        return this->hostSocket;
    }

    void start();
protected:
    boost::asio::ip::tcp::socket hostSocket;
    lkl::asio::ip::tcp::socket remoteSocket;

    SocksConnection(boost::asio::io_service &io_service);
};

#endif /* SOCKSCONNECTION_H_ */
