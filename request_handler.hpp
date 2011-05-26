/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_HANDLER_HPP_
#define SHIRITORI_REQUEST_HANDLER_HPP_

#include <string>
#include <sstream>
#include <vector>

namespace shiritori {

struct request_handler
{
	virtual ~request_handler() {}
	virtual std::string get_response(std::string const &request) const = 0;
};

struct command_unknown_error
	: public request_handler
{
	std::string get_response(std::string const &request) const
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
	std::vector<std::string> &history_;
public:
	identical_response(std::vector<std::string> &history)
		: history_(history) {}
	virtual ~identical_response() {}
	std::string get_response(std::string const &request) const
	{
		history_.push_back(request);
		return request + "\r\n";
	}
};

template<std::size_t HISTORY_SIZE>
class show_history
	: public request_handler
{
	std::vector<std::string> &history_;
public:
	show_history(std::vector<std::string> &history)
		: history_(history) {}
	virtual ~show_history() {}
	std::string get_response(std::string const &request) const
	{
		if (history_.empty()) return "no history.\r\n";
		std::string result;
		std::for_each(
			std::max(history_.begin(), history_.end() - HISTORY_SIZE),
			history_.end(),
			[&result](std::string const &string){ result.append(string); result.append("\r\n"); });
		return result;
	}
};

class reset_history
	: public request_handler
{
	std::vector<std::string> &history_;
public:
	reset_history(std::vector<std::string> &history)
		: history_(history) {}
	virtual ~reset_history() {}
	std::string get_response(std::string const &request) const
	{
		history_.clear();
		return "reset done.\r\n";
	}
};

struct word_unkonw_error
	: public request_handler
{
	virtual ~word_unkonw_error() {}
	std::string get_response(std::string const &request) const
	{
		return "unknown word: " + request;
	}
};

class not_begin_with_previous_tail_error
	: public request_handler
{
	const std::vector<std::string> &history_;
public:
	not_begin_with_previous_tail_error(std::vector<std::string> &history)
		: history_(history) {}
	~not_begin_with_previous_tail_error() {}
	std::string get_response(std::string const &request) const
	{
		assert(!history_.empty());
		std::string const previous_word(history_[history_.size() - 1]);

		std::stringstream ss;
		ss << request << " does not follow "
		   << previous_word << "\r\n";
		return ss.str();
	}
};

} // namespace shiritori

#endif // SHIRITORI_REQUEST_HANDLER_HPP_
