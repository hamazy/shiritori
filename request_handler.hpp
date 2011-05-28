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
	virtual void respond_to(std::set<player::pointer> &, player::pointer requesting_player, std::string const &response)
	{
		requesting_player->deliver(response);
	}
	virtual void notify_all(std::set<player::pointer> &players, std::string const &response)
	{
		std::for_each(
			players.begin(), players.end(),
			boost::bind(&player::deliver, _1, boost::ref(response)));
	}
};

struct command_unknown_error
	: public request_handler
{
	std::string get_response(std::string const &request) const
	{
		std::string command(request); // TODO
		std::stringstream ss;
		ss << ">> unknown command" << command << "\r\n";
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
	std::string get_response(std::string const &request) const
	{
		history_.push_back(request);
		return std::string("=> ") + request + "\r\n";
	}
	void respond_to(std::set<player::pointer> &players, player::pointer, std::string const &response)
	{
		notify_all(players, response);
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
	std::string get_response(std::string const &) const
	{
		if (history_.empty()) return ">> no history.\r\n";
		std::string result;
		std::for_each(
			std::max(history_.begin(), history_.end() - HISTORY_SIZE),
			history_.end(),
			[&result](std::string const &string){ result.append(std::string(">> ") + string + "\r\n"); });
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
	std::string get_response(std::string const &) const
	{
		history_.clear();
		return ">> reset done.\r\n";
	}
	void respond_to(std::set<player::pointer> &players, player::pointer, std::string const &response)
	{
		notify_all(players, response);
	}
};

struct word_unkonw_error
	: public request_handler
{
	std::string get_response(std::string const &request) const
	{
		return ">> unknown word: " + request;
	}
};

class not_begin_with_previous_tail_error
	: public request_handler
{
	const std::vector<std::string> &history_;
public:
	not_begin_with_previous_tail_error(std::vector<std::string> &history)
		: history_(history) {}
	std::string get_response(std::string const &request) const
	{
		assert(!history_.empty());
		std::string const previous_word(history_[history_.size() - 1]);

		std::stringstream ss;
		ss << ">> \"" << request << "\" does not follow \""
		   << previous_word << "\"\r\n";
		return ss.str();
	}
};

class not_unique_word_error
	: public request_handler
{
public:
	std::string get_response(std::string const &request) const
	{
		return std::string(">> \"") + request + "\" is already appeared.\r\n";
	}
};

class empty_request_error
	: public request_handler
{
public:
	std::string get_response(std::string const &) const
	{
		return std::string(">> empty request.\r\n");
	}
};

} // namespace shiritori

#endif // SHIRITORI_REQUEST_HANDLER_HPP_
