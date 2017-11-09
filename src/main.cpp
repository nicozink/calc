#include <fstream>
#include <iostream>

#include <calc_parser.hpp>

extern "C" FILE *c_comp_in;

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		std::string param(argv[1]);

		FILE* input = fopen(param.c_str(), "r");
		if (!input)
		{
		   std::cout << "Could not open file to parse." << std::endl; 
		   return -1;
		} 
	 
		c_comp_in = input;
	 
		do 
		{
			c_comp_parse();
		} while (!feof(c_comp_in));
	}
	else
	{
		c_comp_parse();
	}

	return 0;
}
