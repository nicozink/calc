/*
Copyright � Nico Zink
All rights reserved.
*/

#pragma once

// Local includes

// Project includes

// System Includes
#include <map>

class ValueToId
{
public:

	typedef int value_id;

	template<typename Type>
	value_id get_id(Type value)
	{
		static value_id next_id = 0;
		static std::map<Type, value_id> mapped_ids;

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

/*template<typename Type, typename IdType>
class IdsPerType
{
public:

	static IdType get_id(Type value)
	{
		static IdType next_id = 0;

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

private:

	static std::map<Type, IdType> mapped_ids;
};

template<typename Type, typename IdType>
std::map<Type, IdType> ValueToId::mapped_ids;*/