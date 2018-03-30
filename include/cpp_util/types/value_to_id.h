/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "type_to_id.h"
#include "variant_map.h"

// Project includes

// System Includes
#include <map>

class ValueToId
{
public:

	typedef int value_id;

	ValueToId();

	template<typename Type>
	value_id get_id(Type value);

	template<typename Type>
	std::map<Type, value_id>& get_id_lookup();

private:

	TypeToId::type_id id_counter;

	VariantMap<TypeToId::type_id> type_ids;
};

template<typename Type>
ValueToId::value_id ValueToId::get_id(Type value)
{
	std::map<Type, ValueToId::value_id>& id_lookup = get_id_lookup<Type>();

	if (id_lookup.find(value) == id_lookup.end())
	{
		id_lookup[value] = id_counter++;
	}

	return id_lookup[value];
}

template<typename Type>
std::map<Type, ValueToId::value_id>& ValueToId::get_id_lookup()
{
	TypeToId::type_id type_id = TypeToId::get_id<Type>(); 
	
	if (type_ids.find(type_id) == type_ids.end())
	{
		type_ids.insert<std::map<Type, ValueToId::value_id>>({ type_id, std::map<Type, ValueToId::value_id>() });
	}

	return type_ids.get<std::map<Type, ValueToId::value_id>>(type_id);
}
