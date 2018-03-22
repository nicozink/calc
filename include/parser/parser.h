/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "parse_result.h"

// Project includes
#include <cpp_util/types/value_to_id.h>
#include <cpp_util/types/variant_list.h>

// System Includes
#include <functional>
#include <string>

class parser
{
    public:

    parse_result parse(std::istream& input);

	parse_result parse(const std::string str);

	parse_result parse_file(const std::string path);
};

typedef int token_id;

template <typename TokenType>
class production
{
public:

	production(token_id id);

	template <typename ValueType>
	production<TokenType>& execute(std::function<ValueType(VariantList&)> func);

	production<TokenType>& read_token(TokenType token_type);

	template <typename ReadType>
	production<TokenType>& read_type(std::string regex);

	template <typename ReadType>
	production<TokenType>& read_value(ReadType value);

private:

	token_id id;
};

template <typename TokenType>
production<TokenType>::production(token_id id)
{
	this->id = id;
}

template <typename TokenType>
class grammar
{
public:

	production<TokenType>& add_production(TokenType type);

    parser generate();

private:

	std::vector<production<TokenType>> productions;
};

template <typename TokenType>
production<TokenType>& grammar<TokenType>::add_production(TokenType type)
{
	token_id id = ValueToId<TokenType, token_id>::get_id(type);
	productions.push_back(production<TokenType>(id));

    return productions.back();
}

template <typename TokenType>
parser grammar<TokenType>::generate()
{
    return parser();
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_token(TokenType token_type)
{
    return *this;
}

template <typename TokenType>
template <typename ReadType>
production<TokenType>& production<TokenType>::read_type(std::string regex)
{
    return *this;
}

template <typename TokenType>
template <typename ReadType>
production<TokenType>& production<TokenType>::read_value(ReadType value)
{
    return *this;
}

template <typename TokenType>
template <typename ValueType>
production<TokenType>& production<TokenType>::execute(std::function<ValueType(VariantList&)> func)
{
    return *this;
}
