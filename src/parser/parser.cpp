/*
Copyright © Nico Zink
All rights reserved.
*/

#include <parser/parser.h>

// Local includes

// Project includes

// System Includes
#include <fstream>
#include <istream>
#include <sstream>

Parser::Parser(const ParserData& pd)
{

}

parse_result Parser::parse(std::istream& input)
{
	VariantList v;
	v.push_back<int>(100);
	
	return parse_result(v);
}

parse_result Parser::parse(const std::string str)
{
	std::istringstream stream(str);
	return parse(stream);
}

parse_result Parser::parse_file(const std::string path)
{
	std::ifstream file(path.c_str());
	return parse(file);
}
