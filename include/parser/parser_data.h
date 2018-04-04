/*
Copyright (c) Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "production_data.h"
#include "token_data.h"

// Project includes

// System Includes

struct ParserData
{
	std::vector<ProductionData> productions;

	std::vector<TokenData> tokens;
};