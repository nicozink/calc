%{
#include <stdlib.h>
#include <string>
void yyerror(char*);
#include "calc_parser.hpp"
#include "calc.h"
%}

%%

"print" {
	return PRINT;
}

[0-9]\.?[0-9]* {
	yylval.NUM = atof(yytext);
	return NUM;
}

[a-zA-Z_][a-zA-Z0-9]* {
	yylval.IDENTIFIER = strdup(yytext);

	return IDENTIFIER;
}

[-+*/=(){};] {
	return *yytext;
}

[\t\n] {
	/* skip whitespace */
}

. {
	yyerror("invalid charactr");
}

%%

int yywrap(void) {
	return 1;
}

