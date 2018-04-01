/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes

// Project includes

// System Includes

class TypeToId
{
public:

	typedef int type_id;

	TypeToId();

	template<typename Type>
	static type_id get_id();

	static type_id get_next_id();

private:

	static type_id type_counter;
};

template<typename Type>
TypeToId::type_id TypeToId::get_id()
{
	static const TypeToId::type_id id = get_next_id();

	return id;
}
