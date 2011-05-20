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

} // namespace shiritori

#endif // SHIRITORI_REQUEST_HANDLER_HPP_
