/*
Copyright (c) Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <parser/parser_generator.h>
#include <unittest/test.h>

// External Includes

TEST(Parser, TestSimpleNumber)
{
	enum class Token
    {
        value
    };

    ParserGenerator<Token> g;

	g.add_production(Token::value)
		.read_type<int>("[1-9][0-9]*")
		.execute<int>([](VariantList &v) {
			auto value = v.get<int>(0);
			return value;
		});
    
    Parser p = g.generate();

    ASSERT(p.parse("100").get<int>() == 100);
}

TEST(Parser, TestAddition)
{
	enum class Token
	{
		value,
		addition
	};

	ParserGenerator<Token> g;

	g.add_production(Token::addition)
		.read_token(Token::value)
		.read_value('+')
		.read_token(Token::value)
		.execute<int>([](VariantList &v) {
			auto v1 = v.get<int>(0);
			auto v2 = v.get<int>(2);
			return v1 + v2;
	});

	g.add_production(Token::value)
		.read_type<int>("[1-9][0-9]*")
		.execute<int>([](VariantList &v) {
			auto value = v.get<int>(0);
			return value;
	});

	Parser p = g.generate();

	ASSERT(p.parse("15+20").get<int>() == 35);
}