/*
Copyright © Nico Zink
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

	l.add_token("[1-9][0-9]*");
	l.add_token(",");

	l.parse("100,200");

	ASSERT(l.get_next().compare("100") == 0);
	ASSERT(l.get_next().compare(",") == 0);
	ASSERT(l.get_next().compare("200") == 0);
}
