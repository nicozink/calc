/*
Copyright (c) Nico Zink
All rights reserved.
*/

#include <parser/parser.h>

// Local includes

// Project includes

// System Includes
#include <fstream>
#include <istream>
#include <sstream>

Parser::Parser(const ParserData& pd)
{
	for (auto& token : pd.tokens)
	{
		lex.add_token(token.get_id(), token.get_regex());
		token_lookup.insert({ token.get_id(), token });
	}
}

VariantType Parser::parse(std::istream& input)
{
	VariantType v;

	lex.parse(input);

	while (lex.has_next())
	{
		auto match = lex.get_next();

		TokenData& td = token_lookup[match.first];
		td.get_execute()(v, match.second);
	}

	return v;
}

VariantType Parser::parse(const std::string str)
{
	std::istringstream stream(str);
	return parse(stream);
}

VariantType Parser::parse_file(const std::string path)
{
	std::ifstream file(path.c_str());
	return parse(file);
}
