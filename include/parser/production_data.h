/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "token_id.h"

// Project includes
#include <cpp_util/types/variant_list.h>
#include <cpp_util/types/variant_type.h>

// System Includes
#include <functional>
#include <vector>

class ProductionData
{
public:

	ProductionData();

	void add_symbol(token_id symbol_id);

	std::function<void(VariantType&, VariantList&)>& get_execute();

	std::vector<token_id>& get_symbols();

	token_id get_id();

	void set_execute(std::function<void(VariantType&, VariantList&)> func);

	void set_id(token_id id);

private:

	token_id id;

	std::vector<token_id> symbols;

	std::function<void(VariantType&, VariantList&)> read_function;
};
