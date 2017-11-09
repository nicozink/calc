%{
#include <stdlib.h>
#include <string>
void c_comp_error(char*);
#include "calc_parser.hpp"
#include "calc.h"
%}

%option prefix="c_comp_"

%%

"print" {
	return PRINT;
}

[0-9]\.?[0-9]* {
	c_comp_lval.NUM = atof(c_comp_text);
	return NUM;
}

[a-zA-Z_][a-zA-Z0-9]* {
	c_comp_lval.IDENTIFIER = strdup(c_comp_text);

	return IDENTIFIER;
}

[-+*/=(){};] {
	return *c_comp_text;
}

[\t\n] {
	/* skip whitespace */
}

. {
	c_comp_error("invalid charactr");
}

%%

int c_comp_wrap(void) {
	return 1;
}

