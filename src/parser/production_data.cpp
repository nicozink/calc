/*
Copyright © Nico Zink
All rights reserved.
*/

#include <parser/production_data.h>

// Local includes

// Project includes

// System Includes

ProductionData::ProductionData()
{

}

void ProductionData::add_symbol(token_id symbol_id)
{
	symbols.push_back(symbol_id);
}

std::function<void(VariantType&, VariantList&)>& ProductionData::get_execute()
{
	return read_function;
}

std::vector<token_id>& ProductionData::get_symbols()
{
	return symbols;
}

token_id ProductionData::get_id()
{
	return id;
}

void ProductionData::set_execute(std::function<void(VariantType&, VariantList&)> func)
{
	read_function = func;
}

void ProductionData::set_id(token_id id)
{
	this->id = id;
}
