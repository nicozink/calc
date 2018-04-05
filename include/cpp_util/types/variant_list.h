/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "variant_type.h"

// Project includes

// System Includes
#include <vector>

class VariantList : public std::vector<VariantType>
{
public:

    VariantList();

    ~VariantList();
    
    template <typename T>
	T& get(int i);

    template <typename T>
	void push_back_type(const T& item);
};

template <typename T>
T& VariantList::get(int i)
{
	return (*this)[i].get<T>();
}

template <typename T>
void VariantList::push_back_type(const T& item)
{
    VariantType type;
	type.set(item);

	push_back(std::move(type));
}
