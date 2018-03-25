/*
Copyright © Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <cpp_util/types/variant_type.h>
#include <unittest/test.h>

// External Includes
#include <iostream>

TEST(VariantType, TestSimpleInteger)
{
	VariantType test_type;

	int i = 42;

	test_type.set(i);

	ASSERT(test_type.get<int>() == i);
}

TEST(VariantType, TestSimpleDouble)
{
	VariantType test_type;

	double i = 42.5;

	test_type.set(i);

	ASSERT(test_type.get<double>() == i);
}

TEST(VariantType, TestString)
{
	VariantType test_type;

	std::string str("42");
	
	test_type.set(str);

	ASSERT(test_type.get<std::string>().compare(str) == 0);
}
