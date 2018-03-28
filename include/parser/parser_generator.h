/*
Copyright � Nico Zink
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
#include <cpp_util/types/value_to_id.h>
#include <cpp_util/types/variant_list.h>

// System Includes
#include <functional>
#include <string>

template <typename TokenType>
class ParserGenerator
{
public:

	production<TokenType> add_production(TokenType type);

	parser generate();

private:

	Grammar<TokenType> grammar;

	ValueToId id_generator;

	std::vector<ProductionData> productions;
};

template <typename TokenType>
production<TokenType> ParserGenerator<TokenType>::add_production(TokenType type)
{
	productions.emplace_back();
	ProductionData& data = productions.back();

	token_id id = id_generator.get_id<TokenType>(type);
	data.set_id(id);

	return production<TokenType>(data, id_generator);
}

template <typename TokenType>
parser ParserGenerator<TokenType>::generate()
{
	return parser();
}