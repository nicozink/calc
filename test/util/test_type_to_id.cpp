/*
Copyright (c) Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <cpp_util/types/type_to_id.h>
#include <unittest/test.h>

// External Includes

TEST(TypeToId, TestDifferentTypes)
{
	TypeToId::type_id id1 = TypeToId::get_id<int>();
	TypeToId::type_id id2 = TypeToId::get_id<double>();
	TypeToId::type_id id3 = TypeToId::get_id<std::string>();

	ASSERT(id1 != id2);
	ASSERT(id1 != id3);
	ASSERT(id2 != id3);
}

TEST(TypeToId, TestDuplicateTypes)
{
	TypeToId::type_id id1 = TypeToId::get_id<TypeToId::type_id>();
	TypeToId::type_id id2 = TypeToId::get_id<double>();
	TypeToId::type_id id3 = TypeToId::get_id<TypeToId>();
	TypeToId::type_id id4 = TypeToId::get_id<TypeToId::type_id>();
	TypeToId::type_id id5 = TypeToId::get_id<double>();
	TypeToId::type_id id6 = TypeToId::get_id<TypeToId>();

	ASSERT(id1 == id4);
	ASSERT(id2 == id5);
	ASSERT(id3 == id6);
	ASSERT(id1 != id2);
	ASSERT(id1 != id3);
	ASSERT(id2 != id3);
}
