/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "parse_result.h"
#include "parser_data.h"

// Project includes

// System Includes
#include <functional>
#include <string>

class Parser
{
public:

	Parser(const ParserData& pd);

	parse_result parse(std::istream& input);

	parse_result parse(const std::string str);

	parse_result parse_file(const std::string path);
};
