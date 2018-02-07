/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

enum class CalcToken
{
	id_print,
	id_function,
	id_return,
	id_var,
	number,
	identifier,
	round_bracket_open,
	round_bracket_close,
	curly_bracket_open,
	curly_bracket_close,
	equals,
	plus,
	minus,
	multiply,
	divide,
	end_of_input,
};
