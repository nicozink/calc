/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "production_data.h"
#include "token_data.h"

// Project includes
#include <cpp_util/types/value_to_id.h>

// System Includes
#include <string>

struct Grammar
{
public:

	template <typename Type>
	ProductionData& add_production(Type type);	

	template <typename Type>
	TokenData& add_token(std::string regex);

	ValueToId id_generator;

	std::vector<ProductionData> productions;

	std::map<ValueToId::value_id, TokenData> tokens;

private:

	template <typename Type>
	std::function<void(VariantType&, std::string)> get_token_function();
};

template <typename Type>
ProductionData& Grammar::add_production(Type type)
{
	productions.emplace_back();
	ProductionData& data = productions.back();

	ValueToId::value_id id = id_generator.get_id<Type>(type);
	data.set_id(id);

	return data;
}

template <typename Type>
TokenData& Grammar::add_token(std::string regex)
{
	ValueToId::value_id id = id_generator.get_id(regex);
	
	if (tokens.find(id) == tokens.end())
	{
		TokenData data;
		data.set_id(id);
		data.set_regex(regex);
		data.set_execute(get_token_function<Type>());

		tokens.insert({ id, data });
	}

	return tokens[id];
}

template <>
std::function<void(VariantType&, std::string)> Grammar::get_token_function<char>()
{
	return[](VariantType& t, std::string s) {
		t.set(s[0]);
	};
}

template <>
std::function<void(VariantType&, std::string)> Grammar::get_token_function<double>()
{
	return[](VariantType& t, std::string s) {
		t.set(std::stod(s));
	};
}

template <>
std::function<void(VariantType&, std::string)> Grammar::get_token_function<int>()
{
	return[](VariantType& t, std::string s) {
		t.set(std::stoi(s));
	};
}

template <>
std::function<void(VariantType&, std::string)> Grammar::get_token_function<std::string>()
{
	return[](VariantType& t, std::string s) {
		t.set(s);
	};
}

template <>
std::function<void(VariantType&, std::string)> Grammar::get_token_function<void>()
{
	return[](VariantType& t, std::string s) { };
}
