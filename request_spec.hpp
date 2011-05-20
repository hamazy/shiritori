/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_SPEC_HPP_
#define SHIRITORI_REQUEST_SPEC_HPP_

namespace shiritori {

class request_spec
	: public std::unary_function<std::string,bool>
{
public:
	virtual ~request_spec() {}
	virtual bool operator()(std::string const &request) const = 0;
};

class any_request
	: public request_spec
{
public:
	any_request() {}
	virtual ~any_request() {}
	virtual bool operator()(std::string const &request) const
	{
		return true;
	}
private:
	any_request(any_request const &src);
	any_request &operator=(any_request const &src);

};

}

#endif // SHIRITORI_REQUEST_SPEC_HPP_
