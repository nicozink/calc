/*
Copyright (c) Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <parser/lexer.h>
#include <unittest/test.h>

// External Includes

TEST(Lexer, TestSimpleNumber)
{
	Lexer l;

	l.add_token(0, "[1-9][0-9]*");
	l.add_token(1, ",");

	l.parse("100,200");

	auto match1 = l.get_next();
	ASSERT(match1.first == 0);
	ASSERT(match1.second.compare("100") == 0);

	auto match2 = l.get_next();
	ASSERT(match2.first == 1);
	ASSERT(match2.second.compare(",") == 0);
	
	auto match3 = l.get_next();
	ASSERT(match3.first == 0);
	ASSERT(match3.second.compare("200") == 0);
}
