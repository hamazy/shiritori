/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_HANDLER_HPP_
#define SHIRITORI_REQUEST_HANDLER_HPP_

#include <string>
#include <sstream>

namespace shiritori {

struct request_handler
{
	virtual ~request_handler() {}
	virtual std::string get_response(std::string const &request) const = 0;
};

struct command_unknown_error
	: public request_handler
{
	virtual std::string get_response(std::string const &request) const
	{
		std::string command(request); // TODO
		std::stringstream ss;
		ss << "unknown command" << command << "\r\n";
		return  ss.str();
	}
};

struct identical_response
	: public request_handler
{
public:
	virtual ~identical_response() {}
	std::string get_response(std::string const &request) const
	{
		return request + "\r\n";
	}
};

} // namespace shiritori

#endif // SHIRITORI_REQUEST_HANDLER_HPP_
