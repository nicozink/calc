/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "variant_type.h"

// Project includes

// System Includes
#include <vector>

class VariantList
{
    public:

    VariantList();

    ~VariantList();
    
    template <typename T>
	T& get(int i);

    template <typename T>
	void push_back(const T& item);

    int size() const;

    private:
		 
    std::vector<VariantType> items;
};

template <typename T>
T& VariantList::get(int i)
{
	return items[i].get<T>();
}

template <typename T>
void VariantList::push_back(const T& item)
{
    VariantType type;
	type.set(item);

    items.push_back(std::move(type));
}
