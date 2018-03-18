/*
Copyright © Nico Zink
All rights reserved.
*/

#include <parser/lexer.h>

// Local includes

// Project includes

// System Includes
#include <regex>

Lexer::Lexer()
{

}

void Lexer::add_token(std::string regex)
{
	tokens.push_back(regex);
}

void Lexer::parse(std::istream& input)
{
	input_string = std::string(std::istreambuf_iterator<char>(input), {});
}

void Lexer::parse(const std::string str)
{
	input_string = str;
}

std::string Lexer::get_next()
{
	std::string best_match = "";

	for (auto token : tokens)
	{
		std::regex regex("^" + token);

		std::smatch regex_match;
		if (std::regex_search(input_string, regex_match, regex))
		{
			for (size_t i = 0; i < regex_match.size(); ++i)
			{
				std::string match = regex_match[i];

				if (match.length() > best_match.length())
				{
					best_match = match;
				}
			}
		}
	}

	input_string = input_string.substr(best_match.length());
	
	return best_match;
}

bool Lexer::has_next()
{
	return input_string.length() != 0;
}
