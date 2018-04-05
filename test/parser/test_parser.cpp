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

	int result = p.parse("100").get<int>();

    ASSERT(result == 100);
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

TEST(Parser, TestSumAndProduct)
{
	// Implementation of the example given here:
	// https://en.wikipedia.org/wiki/LR_parser

	enum class Token
	{
		goal,
		sum,
		product,
		value
	};

	ParserGenerator<Token> g;

	g.add_production(Token::goal)
		.read_token(Token::sum)
		.execute<int>([](VariantList &v) {
			return v.get<int>(0);
	});

	g.add_production(Token::sum)
		.read_token(Token::sum)
		.read_value('+')
		.read_token(Token::product)
		.execute<int>([](VariantList &v) {
			auto v1 = v.get<int>(0);
			auto v2 = v.get<int>(2);
			return v1 + v2;
	});

	g.add_production(Token::sum)
		.read_token(Token::product)
		.execute<int>([](VariantList &v) {
			return v.get<int>(0);
	});

	g.add_production(Token::product)
		.read_token(Token::product)
		.read_value('*')
		.read_token(Token::value)
		.execute<int>([](VariantList &v) {
			auto v1 = v.get<int>(0);
			auto v2 = v.get<int>(2);
			return v1 * v2;
	});

	g.add_production(Token::product)
		.read_token(Token::value)
		.execute<int>([](VariantList &v) {
			return v.get<int>(0);
	});

	g.add_production(Token::value)
		.read_type<int>("[1-9][0-9]*")
		.execute<int>([](VariantList &v) {
			return v.get<int>(0);
	});

	Parser p = g.generate();

	ASSERT(p.parse("15+20").get<int>() == 35);
	ASSERT(p.parse("15*20").get<int>() == 300);
	ASSERT(p.parse("15*2+20").get<int>() == 50);
	ASSERT(p.parse("15+2*20").get<int>() == 55);
	ASSERT(p.parse("15*2+20*3").get<int>() == 90);
}
