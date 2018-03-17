/*
Copyright © Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <unittest/test.h>

// External Includes
#include <iostream>
#include <regex>

TEST(Regex, TestRegexParse)
{
	std::string s = "Hello";
	std::regex r("^Hello|Hell");

	std::smatch regex_match;
	if (std::regex_search(s, regex_match, r))
	{
		for (size_t i = 0; i < regex_match.size(); ++i) 
		{
			std::cout << i << ": " << regex_match[i] << std::endl;
		}
	}
}
