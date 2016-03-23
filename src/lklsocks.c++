#include <iostream>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "lkl_helper.h"
#include "SocksServer.h"

namespace po = boost::program_options;

static const size_t LKL_MEMORY_SIZE = 10*1024*1024; // 10 MB


namespace boost { namespace asio { namespace ip {

void validate(boost::any &v, const std::vector<std::string> &values,
              boost::asio::ip::address_v4 *target_type,
              int) {
    // Make sure no previous assignment to 'v' was made.
    po::validators::check_first_occurrence(v);
    // Extract the first string from 'values'. If there is more than
    // one string, it's an error, and exception will be thrown.
    const std::string &s = po::validators::get_single_string(values);
    try {
        v = boost::any(boost::asio::ip::address_v4::from_string(s));
    } catch (std::exception &e) {
        throw po::validation_error(po::validation_error::invalid_option_value);
    }
}

}}}


std::shared_ptr<po::variables_map> parse_config(int argc, char* argv[]) {
    po::options_description config("Allowed options");
    config.add_options()(
            "version,v", "print version string")(
            "help,h", "produce help message")(
            "socket,s", po::value<std::string>()->value_name("socketdir")->required(),
                "specify the UNIX socket to be used by local programs for joining the VDE")(
            "remotehost,H", po::value<boost::asio::ip::address_v4>()->value_name("addr")->required(),
                "IP address of the SOCKS server within the VDE network")(
            "netmask,n", po::value<unsigned int>()->value_name("nm")->default_value(24),
                "netmask of the SOCKS server within the VDE network")(
            "listen,l", po::value<boost::asio::ip::address_v4>()->value_name("addr")->default_value(boost::asio::ip::address_v4::any()),
                "IP address the SOCKS server should listen on")(
            "port,p", po::value<unsigned short>()->value_name("port")->default_value(1080),
                "TCP port number the SOCKS server should listen on");

    std::shared_ptr<po::variables_map> vm(new po::variables_map);
    po::store(po::parse_command_line(argc, argv, config), *vm);
    try {
        po::notify(*vm);
    } catch (boost::program_options::required_option &e) {
        std::cerr << "Missing argument: " << e.get_option_name() << std::endl;
        std::cout << config << std::endl;
        exit(0);
    }
    if (vm->count("help")) {
        std::cout << config << std::endl;
        exit(0);
    }
    if (vm->count("version")) {
        std::cout << "0.0" << std::endl;
        exit(0);
    }
    return vm;
}

int main(int argc, char *argv[]) {
    std::shared_ptr<po::variables_map> config = parse_config(argc, argv);

    init_lkl(LKL_MEMORY_SIZE,
             "",
             config->at("socket").as<std::string>().c_str(),
             htonl(config->at("remotehost").as<boost::asio::ip::address_v4>().to_ulong()),
             config->at("netmask").as<unsigned int>());

    boost::asio::io_service io_service;
    boost::asio::signal_set signals(io_service);
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.add(SIGQUIT);
    signals.async_wait([&](boost::system::error_code const &error, int signal_number) {
        io_service.stop();
    });

    {
        SocksServer server(config->at("listen").as<boost::asio::ip::address_v4>(),
                           config->at("port").as<unsigned short>(),
                           io_service);

        io_service.run();
    }

    halt_lkl();

    return 0;
}
