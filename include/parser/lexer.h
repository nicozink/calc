/*
Copyright © Nico Zink
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
	Lexer();

	void add_token(std::string regex);

	void parse(std::istream& input);

	void parse(const std::string str);

	std::string get_next();

	bool has_next();

private:

	std::vector<std::string> tokens;

	int input_position;

	std::string input_string;
};
