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

	ProductionData& data;

	Grammar& grammar;
};

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
	TokenData& token_data = grammar.add_token<char>(std::string{ 1, value });
	data.add_symbol(token_data.get_id());

	return *this;
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_value(std::string value)
{
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
