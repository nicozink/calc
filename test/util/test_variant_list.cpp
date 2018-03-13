/*
Copyright © Nico Zink
All rights reserved.
*/

// Local Includes

// Project Includes
#include <util/variant_list.h>
#include <unittest/test.h>

// External Includes

TEST(VariantList, CreateVariantList)
{
	VariantList test_list;

	ASSERT(test_list.size() == 0);
}
