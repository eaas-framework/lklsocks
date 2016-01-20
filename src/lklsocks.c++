#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void
parse_config(int argc, char* argv[]) {
    po::options_description config("Allowed options");
    config.add_options()(
            "version,v", "print version string")(
            "help,h", "produce help message")(
            "socket,s", po::value<std::string>()->value_name("socketdir"),
                               "specify the UNIX socket to be used by local "\
                               "programs for joining the VDE");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, config), vm);
    if (vm.count("help")) {
        std::cout << config << std::endl;
    }
}

int main(int argc, char *argv[]) {
    parse_config(argc, argv);
    return 0;
}
