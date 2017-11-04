%{
#include <stdlib.h>
void yyerror(char*);
#include "calc_parser.hpp"
#include "calc.h"

static symbol_table symbols;
%}

%%

[0-9]\.?[0-9]* {
	yylval.NUM = atof(yytext);
	return NUM;
}

[a-zA-Z_][a-zA-Z0-9]* {
	std::string identifier(yytext);

	if (symbols.exists(identifier))
	{
		yylval.VAR = symbols.get(identifier);
	}
	else
	{
		yylval.VAR = symbols.add(identifier, 0.0);
	}

	return VAR;
}

[-+*/=()\n] {
	return *yytext;
}

[\t] {
	/* skip whitespace */
}

. {
	yyerror("invalid charactr");
}

%%

int yywrap(void) {
	return 1;
}

