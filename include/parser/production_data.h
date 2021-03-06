/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes

// Project includes
#include <cpp_util/types/value_to_id.h>
#include <cpp_util/types/variant_list.h>
#include <cpp_util/types/variant_type.h>

// System Includes
#include <functional>
#include <vector>

class ProductionData
{
public:

	ProductionData();

	void add_symbol(ValueToId::value_id symbol_id);

	std::function<void(VariantType&, VariantList&)>& get_execute();

	std::vector<ValueToId::value_id>& get_symbols();

	ValueToId::value_id get_id();

	void set_execute(std::function<void(VariantType&, VariantList&)> func);

	void set_id(ValueToId::value_id id);

private:

	ValueToId::value_id id;

	std::vector<ValueToId::value_id> symbols;

	std::function<void(VariantType&, VariantList&)> read_function;
};
