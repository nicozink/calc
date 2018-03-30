/*
Copyright (c) Nico Zink
All rights reserved.
*/

#include <cpp_util/types/type_to_id.h>

// Local includes

// Project includes

// System Includes

TypeToId::TypeToId()
{
	type_counter = 0;
}

TypeToId::type_id TypeToId::get_next_id()
{
	return type_counter++;
}

TypeToId::type_id TypeToId::type_counter = 0;
