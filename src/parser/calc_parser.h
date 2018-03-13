/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "calc_lexer.h"

// Project insludes
#include "calc.h"

// System includes
#include <istream>
#include <string>

class CalcParser
{
public:
	CalcParser(std::istream& input);

	translation_unit parse();

private:

	void match(CalcToken token);

	block_node read_block();

	function_node read_function();

	std::string read_identifier();

	translation_unit read_translation_unit();

	CalcLexer lexer;
};