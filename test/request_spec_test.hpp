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

struct foo1_spec
	: public shiritori::request_spec
{
	bool operator()(std::string const &request) const
	{
		return true;
	}
};


#endif // SHIRITORI_REQUEST_SPEC_TEST_HPP_
