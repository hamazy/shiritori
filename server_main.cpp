#include <cstdlib>

#include "configuration.hpp"
#include "server.hpp"

const char *shiritori::configuration::OPTION_DESCRIPTION = "Options";
const char *shiritori::configuration::OPTION_HELP = "help";
const char *shiritori::configuration::OPTION_DESCRIPTION_HELP = "print usage";
const char *shiritori::configuration::OPTION_PORT = "port";
const char *shiritori::configuration::OPTION_DESCRIPTION_PORT = "listen port";
const unsigned shiritori::configuration::DEFAULT_PORT = 1234;

int main(int argc, const char *argv[])
{
	shiritori::configuration configuration;
	if (!configuration.parse(argc, argv))
	{
		configuration.print_usage();
		return EXIT_FAILURE;
	}

	shiritori::server server(configuration.port());
	server.start();

	return EXIT_SUCCESS;
}
