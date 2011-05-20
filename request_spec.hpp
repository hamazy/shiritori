/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_SPEC_HPP_
#define SHIRITORI_REQUEST_SPEC_HPP_

#include <boost/xpressive/xpressive.hpp>

namespace shiritori {

struct request_spec
	: public std::unary_function<std::string,bool>
{
	virtual ~request_spec() {}
	virtual bool operator()(std::string const &request) const = 0;
};

struct command_request
	: public request_spec
{
	virtual ~command_request() {}
	bool operator()(std::string const &request) const
	{
		if (request.empty()) return false;
		return request[0] == ':';
	}
};

class specific_command_request
	: public request_spec
{
	boost::xpressive::sregex const pattern_;
public:
	specific_command_request(char const *pattern)
		: pattern_(boost::xpressive::sregex::compile(std::string(":") + pattern + std::string(".*"))) {}
	virtual ~specific_command_request() {}
	bool operator()(std::string const &request) const
	{
		boost::xpressive::smatch what;
		return boost::xpressive::regex_match(request, what, pattern_);
	}
};

struct any_request
	: public request_spec
{
	virtual ~any_request() {}
	bool operator()(std::string const &request) const
	{
		return true;
	}
};

}

#endif // SHIRITORI_REQUEST_SPEC_HPP_
