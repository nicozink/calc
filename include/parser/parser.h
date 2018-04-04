/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "lexer.h"
#include "parser_data.h"

// Project includes

// System Includes
#include <functional>
#include <string>

class Parser
{
public:

	Parser(const ParserData& pd);

	VariantType parse(std::istream& input);

	VariantType parse(const std::string str);

	VariantType parse_file(const std::string path);

private:

	Lexer lex;

	std::map<TypeToId::type_id, TokenData> token_lookup;
};
