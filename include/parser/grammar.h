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

struct Grammar
{
public:

	template <typename Type>
	ProductionData& add_production(Type type);	

	ValueToId id_generator;

	std::vector<ProductionData> productions;

	std::map<std::string, TokenData> tokens;
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
