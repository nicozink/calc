%{
#include <stdio.h>
#include "calc.h"
int yylex(void);
void yyerror(char*);
%}

%define api.value.type union
%token <double> NUM
%token <struct calc_symbol*> VAR
%type <double> expr

%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'

%%

program:
	program expr '\n'	{ printf("%.10g\n", $2); }
	|
	;

expr:
	NUM { $$ = $1; }
	| VAR { $$ = $1->value; }
	| VAR '=' expr { $$ = $3; $1->value = $3; }
	| expr '+' expr { $$ = $1 + $3; }
	| expr '-' expr { $$ = $1 - $3; }
	| expr '*' expr { $$ = $1 * $3; }
	| expr '/' expr { $$ = $1 / $3; }
	| '-' expr %prec NEG { $$ = -$2; }
	| '(' expr ')' { $$ = $2; }
	;

%%

void yyerror(char* s)
{
	fprintf(stderr, "%s\n", s);
}

