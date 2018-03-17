/*
Copyright © Nico Zink
All rights reserved.
*/

#include <parser/parser.h>

// Local includes

// Project includes

// System Includes

parse_result parser::parse(std::string str)
{
    VariantList v;
    v.push_back<int>(100);

    return parse_result(v);
}
