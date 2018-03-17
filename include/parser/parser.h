/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "parse_result.h"

// Project includes
#include <parser/parser.h>
#include <cpp_util/types/variant_list.h>

// System Includes
#include <functional>

template <typename TokenType, typename ValueType>
class production
{
	public:

	production<TokenType, ValueType>& execute(std::function<ValueType(VariantList&)> func);

	production<TokenType, ValueType>& read_token(TokenType token_type);

	template <typename ReadType>
	production<TokenType, ValueType>& read_type(std::string regex);

	template <typename ReadType>
	production<TokenType, ValueType>& read_value(ReadType value);

	typedef ValueType TType;
};

class parser
{
    public:

    parse_result parse(std::istream& input);

	parse_result parse(const std::string str);

	parse_result parse_file(const std::string path);
};

template <typename TokenType>
class grammar
{
	public:

	template <typename ValueType>
	production<TokenType, ValueType> add_production(TokenType type);

    parser generate();
};

template <typename TokenType>
template <typename ValueType>
production<TokenType, ValueType> grammar<TokenType>::add_production(TokenType type)
{
    return production<TokenType, ValueType>();
}

template <typename TokenType>
parser grammar<TokenType>::generate()
{
    return parser();
}

template <typename TokenType, typename ValueType>
production<TokenType, ValueType>& production<TokenType, ValueType>::read_token(TokenType token_type)
{
    return *this;
}

template <typename TokenType, typename ValueType>
template <typename ReadType>
production<TokenType, ValueType>& production<TokenType, ValueType>::read_type(std::string regex)
{
    return *this;
}

template <typename TokenType, typename ValueType>
template <typename ReadType>
production<TokenType, ValueType>& production<TokenType, ValueType>::read_value(ReadType value)
{
    return *this;
}

template <typename TokenType, typename ValueType>
production<TokenType, ValueType>& production<TokenType, ValueType>::execute(std::function<ValueType(VariantList&)> func)
{
    return *this;
}
