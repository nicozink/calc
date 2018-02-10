#include <fstream>
#include <iostream>

#include "parser/calc_lexer.h"
#include <calc_parser.hpp>

int main(int argc, char** argv)
{
	try
	{
		if (argc > 1)
		{
			std::string path(argv[1]);
			std::ifstream file(path.c_str());

			CalcLexer cl(file);
			CalcParser::parser p(cl);
			p.parse ();
		}
		else
		{
			throw "argument expected.";
		}
	}
	catch (char const* message)
	{
		std::cout << message << std::endl;
	}

	return 0;
}
