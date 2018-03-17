/*
Copyright © Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <parser/parser.h>
#include <unittest/test.h>

// External Includes

TEST(Parser, TestSimpleNumber)
{
	enum class Token
    {
        value
    };

    grammar<Token> g;

	g.add_production<int>(Token::value)
		.read_type<int>("[1-9][0-9]*")
		.execute([](VariantList &v) {
			auto value = v.get<int>(0);
			return value;
		});
    
    parser p = g.generate();

    ASSERT(p.parse("100").get<int>() == 100);
}
