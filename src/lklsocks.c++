#include <iostream>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "lkl_helper.h"
#include "SocksServer.h"

namespace po = boost::program_options;

static const size_t LKL_MEMORY_SIZE = 10*1024*1024; // 10 MB

void parse_config(int argc, char* argv[]) {
    po::options_description config("Allowed options");
    config.add_options()(
            "version,v", "print version string")(
            "help,h", "produce help message")(
            "socket,s", po::value<std::string>()->value_name("socketdir"),
            "specify the UNIX socket to be used by local "
            "programs for joining the VDE");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, config), vm);
    if (vm.count("help")) {
        std::cout << config << std::endl;
    }
}

int main(int argc, char *argv[]) {
    parse_config(argc, argv);
    init_lkl(LKL_MEMORY_SIZE, "");

    boost::asio::io_service io_service;

    SocksServer server(1234, io_service);

    io_service.run();

    halt_lkl();

    return 0;
}
