/*
Copyright (c) Nico Zink
All rights reserved.
*/

#include <parser/token_data.h>

// Local includes

// Project includes

// System Includes

TokenData::TokenData()
{

}

const std::function<void(VariantType&, std::string)>& TokenData::get_execute() const
{
	return read_function;
}

ValueToId::value_id TokenData::get_id() const
{
	return id;
}

std::string TokenData::get_regex() const
{
	return regex;
}

void TokenData::set_execute(std::function<void(VariantType&, std::string)> func)
{
	read_function = func;
}

void TokenData::set_id(ValueToId::value_id id)
{
	this->id = id;
}

void TokenData::set_regex(std::string regex)
{
	this->regex = regex;
}
