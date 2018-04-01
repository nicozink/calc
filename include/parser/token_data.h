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

class TokenData
{
public:

	TokenData();

	std::function<void(VariantType&)>& get_execute();

	ValueToId::value_id get_id();

	std::string get_regex();

	void set_execute(std::function<void(VariantType&)> func);

	void set_id(ValueToId::value_id id);

	void set_regex(std::string regex);

private:

	ValueToId::value_id id;

	std::string regex;

	std::function<void(VariantType&)> read_function;
};
