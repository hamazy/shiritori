/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_SPEC_HPP_
#define SHIRITORI_REQUEST_SPEC_HPP_

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
	virtual bool operator()(std::string const &request) const
	{
		if (request.empty()) return false;
		return request[0] == ':';
	}
};

struct any_request
	: public request_spec
{
	virtual ~any_request() {}
	virtual bool operator()(std::string const &request) const
	{
		return true;
	}
};

}

#endif // SHIRITORI_REQUEST_SPEC_HPP_
