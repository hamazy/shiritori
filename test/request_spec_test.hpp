/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_REQUEST_SPEC_TEST_HPP_
#define SHIRITORI_REQUEST_SPEC_TEST_HPP_

#include "gtest/gtest.h"

#include "request_spec.hpp"

TEST(specific_command_test, history_command)
{
	using shiritori::specific_command_request;
	specific_command_request history_command_spec("history");

	ASSERT_TRUE(history_command_spec(std::string(":history\r\n")));
	ASSERT_TRUE(history_command_spec(std::string(":history\r")));
	ASSERT_TRUE(history_command_spec(std::string(":history\n")));
	ASSERT_TRUE(history_command_spec(std::string(":history \r\n")));
	ASSERT_TRUE(history_command_spec(std::string(": history \r\n")));
	ASSERT_FALSE(history_command_spec(std::string(":historyy \r\n")));
}

class foo1_spec
	: public shiritori::request_spec
{
	int &arg1_;
public:
	typedef int &constructor_arg1_type;
	foo1_spec(int &arg1)
		: arg1_(arg1) {}
	bool operator()(std::string const &) const
	{
		return true;
	}
};

TEST(not_spec_test, one_argument)
{
	using shiritori::not_;
	int value(0);
	ASSERT_FALSE(not_<foo1_spec>(value)(std::string()));
	ASSERT_TRUE(not_<not_<foo1_spec> >(value)(std::string()));
}

class foo2_spec
	: public shiritori::request_spec
{
	int &arg1_;
	double &arg2_;
public:
	typedef int &constructor_arg1_type;
	typedef double &constructor_arg2_type;
	foo2_spec(int &arg1, double &arg2)
		: arg1_(arg1)
		, arg2_(arg2) {}
	bool operator()(std::string const &) const
	{
		return true;
	}
};

TEST(not_spec_test, two_arguments)
{
	using shiritori::not_;
	int value1(0);
	double value2(0.0);
	ASSERT_FALSE(not_<foo2_spec>(value1, value2)(std::string()));
	ASSERT_TRUE(not_<not_<foo2_spec> >(value1, value2)(std::string()));
}
#endif // SHIRITORI_REQUEST_SPEC_TEST_HPP_
