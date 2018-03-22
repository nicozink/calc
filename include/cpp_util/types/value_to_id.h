/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes

// Project includes

// System Includes
#include <map>

template<typename Type, typename IdType>
class ValueToId
{
public:

	static IdType get_id(Type value)
	{
		static IdType next_id = 0;
		static std::map<Type, IdType> mapped_ids;

		if (mapped_ids.find(value) != mapped_ids.end())
		{
			return mapped_ids[value];
		}
		else
		{
			mapped_ids[value] = next_id;
			++next_id;

			return mapped_ids[value];
		}
	}
};
