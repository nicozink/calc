%{
#include <stdlib.h>
void yyerror(char*);
#include "calc_parser.hpp"
%}

%%

[0-9]\.?[0-9]* {
		yylval.NUM = atof(yytext);
		return NUM;
	}

[-+*/()\n]	return *yytext;

[\t]	; /* skip whitespace */

.	yyerror("invalid charactr");

%%

int yywrap(void) {
	return 1;
}

