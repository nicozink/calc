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

	production(ProductionData& data, ValueToId& id_generator);

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

	ValueToId& id_generator;
};

template <typename TokenType>
production<TokenType>::production(ProductionData& data, ValueToId& id_generator)
	: data{ data}, id_generator{ id_generator }
{
	
}

template <typename TokenType>
production<TokenType>::production(production<TokenType>&& p)
	: data{ p.data }, id_generator{ p.id_generator }
{
	
}

template <typename TokenType>
production<TokenType>& production<TokenType>::read_token(TokenType token_type)
{
	ValueToId::value_id id = id_generator.get_id<TokenType>(token_type);
	data.add_symbol(id);

	return *this;
}

template <typename TokenType>
template <typename ReadType>
production<TokenType>& production<TokenType>::read_type(std::string regex)
{
	ValueToId::value_id id = id_generator.get_id<std::string>(regex);
	data.add_symbol(id);

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
