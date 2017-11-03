%{
#include <stdio.h>
int yylex(void);
void yyerror(char*);
%}

%define api.value.type union
%token <double> NUM
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

