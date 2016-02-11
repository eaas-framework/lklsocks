#ifndef SOCKSCONNECTION_H_
#define SOCKSCONNECTION_H_

#include <memory>
#include <thread>
#include <mutex>
#include <list>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>

#include "src/asio/stream_socket_service.hpp"
#include "asio/ip/tcp.hpp"

class SocksConnection : public std::enable_shared_from_this<SocksConnection> {
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
    void teardown();
protected:
    boost::asio::ip::tcp::socket hostSocket;
    lkl::asio::ip::tcp::socket remoteSocket;

    std::thread hostThread;
    std::thread remoteThread;
    std::mutex mutex;

    void handshake();
    void receiveRemote();
    void receiveHost();
    SocksConnection(boost::asio::io_service &io_service);
};

#endif /* SOCKSCONNECTION_H_ */
