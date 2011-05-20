#include <cstdlib>

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "configuration.hpp"
#include "server.hpp"

char const *shiritori::configuration::OPTION_DESCRIPTION = "Options";
char const *shiritori::configuration::OPTION_HELP = "help";
char const *shiritori::configuration::OPTION_DESCRIPTION_HELP = "print usage";
char const *shiritori::configuration::OPTION_PORT = "port";
char const *shiritori::configuration::OPTION_DESCRIPTION_PORT = "listen port";
unsigned const shiritori::configuration::DEFAULT_PORT = 1234;

int main(int argc, char const * const argv[])
{
	shiritori::configuration configuration;
	if (!configuration.parse(argc, argv))
	{
		configuration.print_usage();
		return EXIT_FAILURE;
	}

	boost::asio::io_service io_service;
	shiritori::server server(configuration.port(), configuration.the_game(), io_service);
	boost::thread service(server.start());

	std::string input;
	while (input != "exit")
	{
		std::cin >> input;
	}
	server.stop();
	service.join();
	return EXIT_SUCCESS;
}
