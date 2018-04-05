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

	productions = pd.productions;
}

VariantType Parser::parse(std::istream& input)
{
	lex.parse(input);

	while (lex.has_next())
	{
		VariantType v;

		auto match = lex.get_next();

		TokenData& td = token_lookup[match.first];
		td.get_execute()(v, match.second);

		parse_stack.push_back({ match.first, v });

		while (reduce());
	}

	return parse_stack[0].second;
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

bool Parser::reduce()
{
	for (auto& production : productions)
	{
		std::vector<ValueToId::value_id>& symbols = production.get_symbols();

		if (parse_stack.size() >= symbols.size())
		{
			bool sublist_matches = true;

			for (int i = symbols.size(); i > 0; --i)
			{
				if (symbols[symbols.size() - i] != parse_stack[parse_stack.size() - i].first)
				{
					sublist_matches = false;
					break;
				}
			}

			if (sublist_matches)
			{
				VariantList vl;

				for (int i = parse_stack.size() - symbols.size(); i < parse_stack.size(); ++i)
				{
					vl.push_back(parse_stack[i].second);
				}

				VariantType t;
				production.get_execute()(t, vl);

				for (int i = 0; i < symbols.size(); ++i)
				{
					parse_stack.pop_back();
				}

				parse_stack.push_back({ production.get_id(), t });

				return true;
			}
		}
	}

	return false;
}
