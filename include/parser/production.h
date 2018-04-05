/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "production_data.h"

// Project includes
#include <cpp_util/types/value_to_id.h>

// System Includes

template <typename TokenType>
class production
{
public:

	production(Grammar& grammar, TokenType type);

	production(production<TokenType>&& p);

	production(production<TokenType>& p) = delete;
	production<TokenType>& operator=(production<TokenType>& p) = delete;

	template <typename ValueType>
	production<TokenType>& execute(std::function<ValueType(VariantList&)> func);

	production<TokenType>& read_token(TokenType token_type);

	template <typename ReadType>
	production<TokenType>& read_type(std::string regex);

	production<TokenType>& read_value(char value);

	production<TokenType>& read_value(std::string value);

private:

	bool is_special_character(char c);

	ProductionData& data;

	Grammar& grammar;
};

template <typename TokenType>
bool production<TokenType>::is_special_character(char c)
{
	std::string special_characters(R"(([\^$.|?*+(){}))");

	if (special_characters.find(c) != std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename TokenType>
production<TokenType>::production(Grammar& grammar, TokenType type)
	: data{ grammar.add_production(type) }, grammar{ grammar }
{
	
}

template <typename TokenType>
production<TokenType>::production(production<TokenType>&& p)
	: data{ p.data }, grammar{ p.grammar }
{
	
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_token(TokenType token_type)
{
	ValueToId::value_id id = grammar.id_generator.get_id<TokenType>(token_type);
	data.add_symbol(id);

	return *this;
}

template <typename TokenType>
template <typename ReadType>
production<TokenType>& production<TokenType>::read_type(std::string regex)
{
	TokenData& token_data = grammar.add_token<ReadType>(regex);
	data.add_symbol(token_data.get_id());

	return *this;
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_value(char value)
{
	std::string regex;

	if (is_special_character(value))
	{
		regex.append(1, '\\');
	}

	regex.append(1, value);

	TokenData& token_data = grammar.add_token<char>(regex);
	data.add_symbol(token_data.get_id());

	return *this;
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_value(std::string value)
{
	std::string regex;

	for (int i = 0; i < value.length(); ++i)
	{
		if (is_special_character(value[i]))
		{
			value.insert(i, '\\');
			++i;
		}
	}

	regex.append(value);

	TokenData& token_data = grammar.add_token<char>(value);
	data.add_symbol(token_data.get_id());

	return *this;
}

template <typename TokenType>
template <typename ValueType>
production<TokenType>& production<TokenType>::execute(std::function<ValueType(VariantList&)> func)
{
	data.set_execute([func](VariantType& t, VariantList &v) {
		t.set(func(v));
	});

	return *this;
}
