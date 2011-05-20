/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_GAME_HPP_
#define SHIRITORI_GAME_HPP_

#include <set>
#include <string>
#include <vector>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/lambda/construct.hpp>

#include "player.hpp"
#include "request_handler.hpp"
#include "request_spec.hpp"

namespace shiritori {

class request_handlers
{
	typedef request_spec* spec_type;
	typedef request_handler* data_type;
	typedef std::pair<spec_type,data_type> value_type;
	typedef std::vector<value_type> container_type;
	container_type request_handlers_;

	struct get_spec
		: public std::unary_function<value_type,spec_type>
	{
		spec_type operator()(value_type const &value) const
		{
			return value.first;
		}
	};
	struct get_handler
		: public std::unary_function<value_type,data_type>
	{
		data_type operator()(value_type const &value) const
		{
			return value.second;
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
	request_handlers()
		: request_handlers_() {}

	virtual ~request_handlers()
	{
		std::for_each(
			boost::make_transform_iterator(request_handlers_.begin(), get_spec()),
			boost::make_transform_iterator(request_handlers_.end(), get_spec()),
			std::default_delete<request_spec>());
		std::for_each(
			boost::make_transform_iterator(request_handlers_.begin(), get_handler()),
			boost::make_transform_iterator(request_handlers_.end(), get_handler()),
			std::default_delete<request_handler>());
	}

	request_handler *find(std::string const &request)
	{
		boost::transform_iterator<get_spec, container_type::iterator> end(boost::make_transform_iterator(request_handlers_.end(), get_spec()));
		boost::transform_iterator<get_spec, container_type::iterator> found_spec(
			std::find_if(
				boost::make_transform_iterator(request_handlers_.begin(), get_spec()),
				end,
				spec_conformed(request)));
		if (found_spec == end) return 0;
		container_type::iterator found_pair(found_spec.base());
		return found_pair->second;
	}

	void add(request_spec *spec, request_handler *handler)
	{
		request_handlers_.push_back(std::make_pair(spec, handler));
	}

private:
	request_handlers(request_handlers const &src);
	request_handlers &operator=(request_handlers const &src);
};

class game
{
	std::set<player::pointer> players_;
	request_handlers request_handlers_;
	std::vector<std::string> history_;
public:
	game()
		: players_()
		, request_handlers_()
		, history_()
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

	void add_request_handler(request_spec *spec, request_handler *handler)
	{
		request_handlers_.add(spec, handler);
	}

	std::vector<std::string> &history()
	{
		return history_;
	}

private:
	game(game const &src);
	game &operator=(game const &src);

};

} // namespace shiritori

#endif // SHIRITORI_GAME_HPP_
