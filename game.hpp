/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_GAME_HPP_
#define SHIRITORI_GAME_HPP_

#include <set>
#include <string>

#include <boost/iterator/transform_iterator.hpp>

#include "player.hpp"
#include "request_handler.hpp"

namespace shiritori {

class request_spec
	: public std::unary_function<std::string,bool>
{
public:
	virtual ~request_spec() {}
	virtual bool operator()(std::string const &request) const = 0;
};

class request_handler_map
{
	typedef std::map<request_spec *,request_handler *> map;
	map request_handlers_;

	struct get_key
		: public std::unary_function<map::value_type,map::key_type>
	{
		map::key_type operator()(const map::value_type &value) const
		{
			return value.first;
		}
	};

	class spec_conformed
		: public std::unary_function<request_spec *,bool>
	{
		std::string const &request_;
	public:
		spec_conformed(std::string const &request)
			: request_(request) {}
		bool operator()(request_spec const *spec) const
		{
			if (spec == 0) return false;
			return (*spec)(request_);
		}
	};

public:
	request_handler_map()
		: request_handlers_() {}
	virtual ~request_handler_map() {}

	request_handler *find(std::string const &request)
	{
		boost::transform_iterator<get_key, map::iterator> end(boost::make_transform_iterator(request_handlers_.end(), get_key()));
		boost::transform_iterator<get_key, map::iterator> found_spec(
			std::find_if(
				boost::make_transform_iterator(request_handlers_.begin(), get_key()),
				end,
				spec_conformed(request)));
		if (found_spec == end) return 0;
		map::iterator found_pair(found_spec.base());
		return found_pair->second;
	}

	void add(request_spec &spec, request_handler &handler)
	{
		request_handlers_.insert(std::make_pair(&spec, &handler));
	}

private:
	request_handler_map(request_handler_map const &src);
	request_handler_map &operator=(request_handler_map const &src);
};

class game
{
	std::set<player::pointer> players_;
	request_handler_map request_handlers_;
public:
	game()
		: players_()
		, request_handlers_()
	{}
	virtual ~game() {}

	void join(player::pointer player)
	{
		players_.insert(player);
	}

	void leave(player::pointer player)
	{
		players_.erase(player);
	}

	void respond_to(std::string const &request)
	{
		request_handler const *handler(request_handlers_.find(request));
		if (handler == 0) return;

		const std::string response(handler->get_response(request));
		std::for_each(
			players_.begin(), players_.end(),
			boost::bind(&player::deliver, _1, boost::ref(response)));
	}

	void add_request_handler(request_spec &spec, request_handler &handler)
	{
		request_handlers_.add(spec, handler);
	}

private:
	game(game const &src);
	game &operator=(game const &src);

};

} // namespace shiritori

#endif // SHIRITORI_GAME_HPP_
