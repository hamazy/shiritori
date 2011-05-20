/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_HANDLER_HPP_
#define SHIRITORI_REQUEST_HANDLER_HPP_

namespace shiritori {

class request_handler
{
public:
	virtual ~request_handler() {}
	virtual std::string get_response(std::string const &request) const = 0;
};

class identical_response
	: public request_handler
{
public:
	identical_response() {}
	virtual ~identical_response() {}
	std::string get_response(std::string const &request) const
	{
		return request + "\r\n";
	}

private:
	identical_response(identical_response const &src);
	identical_response &operator=(identical_response const &src);

};

} // namespace shiritori

#endif // SHIRITORI_REQUEST_HANDLER_HPP_
