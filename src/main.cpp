#include <fstream>
#include <iostream>

#include "interpreter/interpreter.h"

#include "parser/calc_lexer.h"
#include "parser/calc_parser.h"

int main(int argc, char** argv)
{
	try
	{
		if (argc > 1)
		{
			/*std::string path(argv[1]);
			std::ifstream file(path.c_str());

			CalcParser p(file);
			translation_unit unit = p.parse ();

			Interpreter interp;
			interp.add(&unit);
			interp.execute();*/
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
