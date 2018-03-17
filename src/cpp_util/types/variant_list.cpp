/*
Copyright © Nico Zink
All rights reserved.
*/

#include <cpp_util/types/variant_list.h>

// Local includes

// Project includes

// System Includes

VariantList::VariantList()
{

}

VariantList::~VariantList()
{
    items.clear();
}

int VariantList::size() const
{
    return items.size();
}
