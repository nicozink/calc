/*
Copyright (c) Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <cpp_util/types/variant_map.h>
#include <unittest/test.h>

// External Includes
#include <iostream>

TEST(VariantMap, CreateVariantMap)
{
	VariantMap<int> test_map;

	ASSERT(test_map.size() == 0);
}

TEST(VariantMap, TestSimpleInteger)
{
	VariantMap<int> test_map;

	int i = 42;

	test_map.insert<int>({ 5, i });

	ASSERT(test_map.get<int>(5) == i);
}

TEST(VariantMap, TestDoubleInteger)
{
	VariantMap<int> test_map;

	int i1 = 42;
	int i2 = 87;

	test_map.insert<int>({ 5, i1 });
	test_map.insert<int>({ 10, i2 });

	ASSERT(test_map.get<int>(5) == i1 && test_map.get<int>(10) == i2);
}

TEST(VariantMap, TestString)
{
	VariantMap<std::string> test_map;

	std::string str("42");
	
	test_map.insert<std::string>({ "06", str });

	ASSERT(test_map.get<std::string>("06").compare(str) == 0);
}

TEST(VariantMap, TestStringIntCombo)
{
	VariantMap<int> test_map;

	std::string str("42");
	int i = 42;

	test_map.insert<std::string>({ 2, str });
	test_map.insert<int>({ 3, i });

	ASSERT(test_map.get<std::string>(2).compare(str) == 0);
	ASSERT(test_map.get<int>(3) == i);
}
