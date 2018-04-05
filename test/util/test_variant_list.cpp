/*
Copyright (c) Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <cpp_util/types/variant_list.h>
#include <unittest/test.h>

// External Includes
#include <iostream>

TEST(VariantList, CreateVariantList)
{
	VariantList test_list;

	ASSERT(test_list.size() == 0);
}

TEST(VariantList, TestSimpleInteger)
{
	VariantList test_list;

	int i = 42;

	test_list.push_back_type(i);

	ASSERT(test_list.get<int>(0) == i);
}

TEST(VariantList, TestDoubleInteger)
{
	VariantList test_list;

	int i1 = 42;
	int i2 = 87;

	test_list.push_back_type(i1);
	test_list.push_back_type(i2);

	ASSERT(test_list.get<int>(0) == i1 && test_list.get<int>(1) == i2);
}

TEST(VariantList, TestString)
{
	VariantList test_list;

	std::string str("42");
	
	test_list.push_back_type(str);

	ASSERT(test_list.get<std::string>(0).compare(str) == 0);
}

TEST(VariantList, TestStringIntCombo)
{
	VariantList test_list;

	std::string str("42");
	int i = 42;

	test_list.push_back_type(str);
	test_list.push_back_type(i);

	ASSERT(test_list.get<std::string>(0).compare(str) == 0);
	ASSERT(test_list.get<int>(1) == i);
}
