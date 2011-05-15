#ifndef SHIRITORI_CONFIGURATION_HPP
#define SHIRITORI_CONFIGURATION_HPP

#include <string>
#include <boost/program_options.hpp>

namespace shiritori {

class configuration {
	unsigned m_port;
	boost::program_options::options_description m_description;
	boost::program_options::variables_map m_variable_map;

public:
	configuration()
		: m_port(DEFAULT_PORT)
		, m_description(OPTION_DESCRIPTION)
		, m_variable_map()
	{
		namespace po = boost::program_options;
		m_description.add_options()
			(OPTION_HELP, OPTION_DESCRIPTION_HELP)
			(OPTION_PORT, po::value<unsigned>(&m_port)->default_value(DEFAULT_PORT), OPTION_DESCRIPTION_PORT);
	}

	virtual ~configuration() {}

	bool parse(int argc, const char *argv[])
	{
		namespace po = boost::program_options;
		try
		{
			po::store(po::parse_command_line(argc, argv, m_description), m_variable_map);
			po::notify(m_variable_map);

			if (m_variable_map.count(OPTION_HELP)) return false;
			if (!m_variable_map.count(OPTION_PORT)) return true;

			m_port = m_variable_map[OPTION_PORT].as<unsigned>();
		}
		catch (const std::exception &exception)
		{
			std::cerr << exception.what() << std::endl;
			return false;
		}
		return true;
	}

	unsigned port() const { return m_port; }

	void print_usage() const
	{
		std::cout << m_description << std::endl;
	}

private:
	configuration(const configuration &src);
	configuration& operator=(const configuration &src);

	static const char *OPTION_DESCRIPTION;
	static const char *OPTION_HELP;
	static const char *OPTION_DESCRIPTION_HELP;
	static const char *OPTION_PORT;
	static const char *OPTION_DESCRIPTION_PORT;
	static const unsigned DEFAULT_PORT;
};

} // namespace shiritori

#endif	// SHIRITORI_CONFIGURATION_HPP
