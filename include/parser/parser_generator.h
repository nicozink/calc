/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "grammar.h"
#include "parse_result.h"
#include "parser.h"
#include "production.h"
#include "production_data.h"

// Project includes
#include <cpp_util/types/variant_list.h>

// System Includes
#include <functional>
#include <string>

template <typename TokenType>
class ParserGenerator
{
public:

	production<TokenType> add_production(TokenType type);

	Parser generate();

private:

	Grammar grammar;
};

template <typename TokenType>
production<TokenType> ParserGenerator<TokenType>::add_production(TokenType type)
{
	return production<TokenType>(grammar, type);
}

template <typename TokenType>
Parser ParserGenerator<TokenType>::generate()
{
	ParserData pd;

	pd.productions = grammar.productions;

	for (auto it = grammar.tokens.begin(); it != grammar.tokens.end(); ++it)
	{
		pd.tokens.push_back(it->second);
	}

	return Parser(pd);
}