/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "token_id.h"

// Project includes

// System Includes

template <typename TokenType>
class production
{
public:

	production(token_id id);

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

	token_id id;
};

template <typename TokenType>
production<TokenType>::production(token_id id)
{
	this->id = id;
}

template <typename TokenType>
production<TokenType>::production(production<TokenType>&& p)
{
	this->id = p.id;
}