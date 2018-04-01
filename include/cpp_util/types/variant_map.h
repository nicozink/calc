/*
Copyright (c) Nico Zink
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
VariantMap<IndexType>::VariantMap()
{

}

template <typename IndexType>
VariantMap<IndexType>::~VariantMap()
{

}

template <typename IndexType>
template <typename T>
T& VariantMap<IndexType>::get(IndexType i)
{
	VariantType& v = (*this)[i];
	return v.get<T>();
}

template <typename IndexType>
template <typename T>
void VariantMap<IndexType>::insert(std::pair<IndexType, T> pair)
{
	VariantType type;
	type.set(pair.second);

	std::map<IndexType, VariantType>::insert({ pair.first, std::move(type) });
}
