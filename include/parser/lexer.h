/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes

// Project includes

// System Includes
#include <istream>
#include <string>
#include <vector>

class Lexer
{
public:
	typedef int token_id;
	
	Lexer();

	void add_token(token_id id, std::string regex);

	void parse(std::istream& input);

	void parse(const std::string str);

	std::pair<token_id, std::string> get_next();

	bool has_next();

private:

	std::vector<std::pair<token_id, std::string>> tokens;

	int input_position;

	std::string input_string;
};
