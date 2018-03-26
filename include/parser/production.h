/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "production_data.h"
#include "token_id.h"

// Project includes

// System Includes

template <typename TokenType>
class production
{
public:

	production(ProductionData& data);

	production(production<TokenType>&& p);

	production(production<TokenType>& p) = delete;
	production<TokenType>& operator=(production<TokenType>& p) = delete;

	template <typename ValueType>
	production<TokenType>& execute(std::function<ValueType(VariantList&)> func);

	production<TokenType>& read_token(TokenType token_type);

	template <typename ReadType>
	production<TokenType>& read_type(std::string regex);

	template <typename ReadType>
	production<TokenType>& read_value(ReadType value);

private:

	ProductionData& data;
};

template <typename TokenType>
production<TokenType>::production(ProductionData& data)
	: data{ data}
{
	
}

template <typename TokenType>
production<TokenType>::production(production<TokenType>&& p)
	: data{ p.data }
{
	
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_token(TokenType token_type)
{
	token_id id = ValueToId<TokenType, token_id>::get_id(token_type);
	data.add_symbol(id);

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
	data.set_execute([func](VariantType& t, VariantList &v) {
		t.set(func(v));
	});

	return *this;
}
