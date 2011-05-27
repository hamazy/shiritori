/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_SPEC_HPP_
#define SHIRITORI_REQUEST_SPEC_HPP_

#include <boost/xpressive/xpressive.hpp>

namespace shiritori {

class request_spec
	: public std::unary_function<std::string,bool>
{
public:
	typedef void constructor_arg1_type;
	virtual ~request_spec() {}
	virtual bool operator()(std::string const &request) const = 0;
};

template <typename SPEC>
class spec_trait
{
public:
	typedef typename SPEC::constructor_arg1_type constructor_arg1_type;
};

template <typename SPEC1, typename SPEC2>
class and_
	: public request_spec
{
public:
	bool operator()(std::string const &request) const
	{
		if (!SPEC1()(request)) return false;
		return SPEC2()(request);
	}
};

template <typename SPEC1, typename SPEC2>
class or_
	: public request_spec
{
public:
	bool operator()(std::string const &request) const
	{
		if (SPEC1()(request)) return true;
		return SPEC2()(request);
	}
};

template <typename SPEC, typename ARG1 = typename spec_trait<SPEC>::constructor_arg1_type>
class not_
	: public request_spec
{
	ARG1 arg1_;
public:
	typedef ARG1 constructor_arg1_type;
	not_(ARG1 arg1)
		: arg1_(arg1) {}
	bool operator()(std::string const &request) const
	{
		return !SPEC(arg1_)(request);
	}
};

template <typename SPEC>
class not_<SPEC, void>
	: public request_spec
{
public:
	bool operator()(std::string const &request) const
	{
		return !SPEC()(request);
	}
};

struct command_request
	: public request_spec
{
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
		: pattern_(boost::xpressive::sregex::compile(std::string(":\\s*") + pattern + std::string("\\W*"))) {}
	bool operator()(std::string const &request) const
	{
		boost::xpressive::smatch what;
		return boost::xpressive::regex_match(request, what, pattern_);
	}
};

struct any_request
	: public request_spec
{
	bool operator()(std::string const &) const
	{
		return true;
	}
};

class known_word_request
	: public request_spec
{
public:
	bool operator()(std::string const &request) const
	{
		return true;			// TODO
	}
};

struct empty_request
	: public request_spec
{
	bool operator()(std::string const &request) const
	{
		return request.empty();
	}	
};

class begin_with_previous_tail
	: public request_spec
{
	std::vector<std::string> const &history_;
public:
	typedef const std::vector<std::string> &constructor_arg1_type;

	begin_with_previous_tail(std::vector<std::string> const &history)
		:history_(history) {}
	bool operator()(std::string const &request) const
	{
		assert(!request.empty());
		if (history_.empty()) return true;
		std::string const previous_word(history_.back());
		if (previous_word.empty()) return true;
		char const previous_word_last_char(previous_word[previous_word.size() - 1]);
		char const first_char(request[0]);
		return previous_word_last_char == first_char;
	}
};

class unique_word_request
	: public request_spec
{
	std::vector<std::string> const &history_;
public:
	typedef const std::vector<std::string> &constructor_arg1_type;

	unique_word_request(std::vector<std::string> const &history)
		: history_(history) {}
	bool operator()(std::string const &request) const
	{
		return history_.end() == find_if(
			history_.begin(), history_.end(),
			std::bind1st(std::equal_to<std::string>(), request));
	}
};

}

#endif // SHIRITORI_REQUEST_SPEC_HPP_
