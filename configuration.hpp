#ifndef SHIRITORI_CONFIGURATION_HPP
#define SHIRITORI_CONFIGURATION_HPP

#include <string>
#include <boost/program_options.hpp>

namespace shiritori {

class configuration {
	unsigned port_;
	boost::program_options::options_description description_;
	boost::program_options::variables_map map_;

public:
	configuration()
		: port_(DEFAULT_PORT)
		, description_(OPTION_DESCRIPTION)
		, map_()
	{
		namespace po = boost::program_options;
		description_.add_options()
			(OPTION_HELP, OPTION_DESCRIPTION_HELP)
			(OPTION_PORT, po::value<unsigned>(&port_)->default_value(DEFAULT_PORT), OPTION_DESCRIPTION_PORT);
	}

	virtual ~configuration() {}

	bool parse(int argc, char const * const argv[])
	{
		namespace po = boost::program_options;
		try
		{
			po::store(po::parse_command_line(argc, argv, description_), map_);
			po::notify(map_);

			if (map_.count(OPTION_HELP)) return false;
			if (!map_.count(OPTION_PORT)) return true;

			port_ = map_[OPTION_PORT].as<unsigned>();
		}
		catch (const std::exception &exception)
		{
			std::cerr << exception.what() << std::endl;
			return false;
		}
		return true;
	}

	unsigned port() const { return port_; }

	void print_usage() const
	{
		std::cout << description_ << std::endl;
	}

private:
	configuration(configuration const &src);
	configuration& operator=(configuration const &src);

	static char const*OPTION_DESCRIPTION;
	static char const*OPTION_HELP;
	static char const*OPTION_DESCRIPTION_HELP;
	static char const*OPTION_PORT;
	static char const*OPTION_DESCRIPTION_PORT;
	static unsigned const DEFAULT_PORT;
};

} // namespace shiritori

#endif	// SHIRITORI_CONFIGURATION_HPP
