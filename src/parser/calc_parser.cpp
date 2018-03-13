/*
Copyright © Nico Zink
All rights reserved.
*/

#include "calc_parser.h"

#include "calc.h"

// Local Includes

#define abc() block

CalcParser::CalcParser(std::istream& input)
	: lexer{ input }
{
	std::string name = read_identifier();
	parameter_list_node params;
	block_node block = read_block();

	function_node function(name, params, block);
	function.abc();
}

translation_unit CalcParser::parse()
{
	lexer.read_next();

	translation_unit unit = read_translation_unit();

	match(CalcToken::end_of_input);

	return unit;
}

void CalcParser::match(CalcToken token)
{
	if (lexer.get_current_token() != token)
	{
		throw "Encountered wrong token.";
	}

	lexer.read_next();
}

block_node CalcParser::read_block()
{
	match(CalcToken::curly_bracket_open);
	
	//statement_list_node statements = read_statement_list();

	//block_node block(name, params, block);

	match(CalcToken::curly_bracket_close);

	//return block;
}

function_node CalcParser::read_function()
{
	match(CalcToken::id_function);
	
	std::string name = read_identifier();
	parameter_list_node params;
	block_node block = read_block();

	function_node function(name, params, block);
	return function;
}

std::string CalcParser::read_identifier()
{
	std::string str = lexer.get_string();

	match(CalcToken::identifier);
	
	return str;
}

translation_unit CalcParser::read_translation_unit()
{
	translation_unit unit;

	while (lexer.get_current_token() == CalcToken::id_function)
	{
		unit.add(read_function());
	}

	return unit;
}
