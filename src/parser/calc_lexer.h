/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "calc_token.h"

// Project includes

// System Includes
#include <istream>

class CalcLexer
{
public:
	CalcLexer(std::istream& input);

	CalcToken get_current_token();

	double get_number();

	std::string get_string();

	CalcToken read_next();

private:

	void read(std::string str);

	void read_char();

	void read_number();

	void read_identifier();

	void skip_char();

	void skip_whitespace();

private:

	int current_char;
	
	CalcToken current_token;

	std::string current_string;
	
	std::istream& input;
};