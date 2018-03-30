/*
Copyright (c) Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <cpp_util/types/value_to_id.h>
#include <unittest/test.h>

// External Includes

TEST(ValueToId, TestDifferentValues)
{
	ValueToId id_generator;

	ValueToId::value_id id1 = id_generator.get_id<int>(1);
	ValueToId::value_id id2 = id_generator.get_id<int>(2);
	ValueToId::value_id id3 = id_generator.get_id<std::string>("3");

	ASSERT(id1 != id2);
	ASSERT(id1 != id3);
	ASSERT(id2 != id3);
}

TEST(ValueToId, TestDuplicateValues)
{
	ValueToId id_generator;

	ValueToId::value_id id1 = id_generator.get_id<TypeToId::type_id>(0);
	ValueToId::value_id id2 = id_generator.get_id<double>(0.0);
	ValueToId::value_id id3 = id_generator.get_id<std::string>("3");
	ValueToId::value_id id4 = id_generator.get_id<TypeToId::type_id>(0);
	ValueToId::value_id id5 = id_generator.get_id<double>(0.0);
	ValueToId::value_id id6 = id_generator.get_id<std::string>("3");

	ASSERT(id1 == id4);
	ASSERT(id2 == id5);
	ASSERT(id3 == id6);
	ASSERT(id1 != id2);
	ASSERT(id1 != id3);
	ASSERT(id2 != id3);
}
