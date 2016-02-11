#include <iostream>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "lkl_helper.h"
#include "SocksServer.h"

namespace po = boost::program_options;

static const size_t LKL_MEMORY_SIZE = 10*1024*1024; // 10 MB

std::shared_ptr<po::variables_map> parse_config(int argc, char* argv[]) {
    po::options_description config("Allowed options");
    config.add_options()(
            "version,v", "print version string")(
            "help,h", "produce help message")(
            "socket,s", po::value<std::string>()->value_name("socketdir")->required(),
                                 "specify the UNIX socket to be used by local "
                                 "programs for joining the VDE")(
            "port,p", po::value<int>()->default_value(1080), "listening port to use");
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

    init_lkl(LKL_MEMORY_SIZE, "", config->at("socket").as<std::string>().c_str());

    boost::asio::io_service io_service;

    SocksServer server(config->at("port").as<int>(), io_service);

    io_service.run();

    halt_lkl();

    return 0;
}
