/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "variant_type.h"

// Project includes

// System Includes
#include <map>

template <typename IndexType>
class VariantMap : public std::map<IndexType, VariantType>
{
public:

	VariantMap();

	~VariantMap();

	template <typename T>
	T& get(IndexType i);

	template <typename T>
	void insert(std::pair<IndexType, T> pair);
};

template <typename IndexType>
VariantMap<typename IndexType>::VariantMap()
{

}

template <typename IndexType>
VariantMap<typename IndexType>::~VariantMap()
{

}

template <typename T>
T& get(IndexType i)
{
	return (*this)[i].get<T>();
}

template <typename T>
void insert(std::pair<IndexType, T> pair)
{
	VariantType type;
	type.set(pair.second);

	std::map<VariantType>::insert({ pair.first, std::move(type) });
}
