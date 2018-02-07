/*
Copyright © Nico Zink
All rights reserved.
*/

#include "calc_lexer.h"

// System includes
#include <sstream>

CalcLexer::CalcLexer(std::istream& input)
	: input{ input }
{
	skip_char();
}

CalcToken CalcLexer::get_current_token()
{
	return current_token;
}

double CalcLexer::get_number()
{
	return stod(current_string);
}

std::string CalcLexer::get_string()
{
	return current_string;
}

CalcToken CalcLexer::read_next()
{
	skip_whitespace();

	if (current_char == 0)
	{
		current_token = CalcToken::end_of_input;
		current_string = "eof";
	}
	if (current_char == '+')
	{
		current_token = CalcToken::plus;
		read_char();
	}
	else if (current_char == '-')
	{
		current_token = CalcToken::minus;
		read_char();
	}
	else if (current_char == '*')
	{
		current_token = CalcToken::multiply;
		read_char();
	}
	else if (current_char == '/')
	{
		current_token = CalcToken::divide;
		read_char();
	}
	else if (current_char == '=')
	{
		current_token = CalcToken::equals;
		read_char();
	}
	else if (current_char == '(')
	{
		current_token = CalcToken::round_bracket_open;
		read_char();
	}
	else if (current_char == ')')
	{
		current_token = CalcToken::round_bracket_close;
		read_char();
	}
	else if (current_char == '{')
	{
		current_token = CalcToken::curly_bracket_open;
		read_char();
	}
	else if (current_char == '}')
	{
		current_token = CalcToken::curly_bracket_close;
		read_char();
	}
	else if ((current_char >= '0' && current_char <= '9') ||
		current_char == '-')
	{
			current_token = CalcToken::number;
			read_number();
	}
	else if ((current_char >= 'a' && current_char <= 'z') ||
		(current_char >= 'A' && current_char <= 'Z') ||
		current_char == '_')
	{
		read_identifier();

		if (current_string == "print")
		{
			current_token = CalcToken::id_print;
		}
		else if (current_string == "function")
		{
			current_token = CalcToken::id_function;
		}
		else if (current_string == "return")
		{
			current_token = CalcToken::id_return;
		}
		else if (current_string == "var")
		{
			current_token = CalcToken::id_var;
		}
		else
		{
			current_token = CalcToken::identifier;
		}
	}

	return current_token;
}

void CalcLexer::read(std::string str)
{
	for (auto c : str)
	{
		if (current_char == c)
		{
			skip_char();
		}
		else
		{
			throw "Unexpected character in token.";
		}
	}

	current_string = str;
}

void CalcLexer::read_char()
{
	current_string = std::string(1, current_char);

	skip_char();
}

void CalcLexer::read_number()
{
	std::string result = "";

	while (
		current_char == '0' ||
		current_char == '1' ||
		current_char == '2' ||
		current_char == '3' ||
		current_char == '4' ||
		current_char == '5' ||
		current_char == '6' ||
		current_char == '7' ||
		current_char == '8' ||
		current_char == '9' ||
		current_char == '-' ||
		current_char == '.')
	{
		result += current_char;

		skip_char();
	}

	current_string = result;
}

void CalcLexer::read_identifier()
{
	std::string result = "";

	while ((current_char >= 'a' && current_char <= 'z') ||
		(current_char >= 'A' && current_char <= 'Z') ||
		(current_char >= '0' && current_char <= '9') ||
		current_char == '_')
	{
		result += current_char;

		skip_char();
	}

	current_string = result;
}

void CalcLexer::skip_char()
{
	current_char = input.get();
	
	if (input.eof())
	{
		current_char = 0;
	}
}

void CalcLexer::skip_whitespace()
{
	while (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r')
	{
		skip_char();
	}
}
