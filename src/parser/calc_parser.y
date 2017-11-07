%{
#include <stdio.h>
#include <string>
#include "calc.h"
int yylex(void);
void yyerror(char*);
%}

%define api.value.type union
%token <double> NUM
%token <char*> IDENTIFIER
%type <struct tree_node*> expr

%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'

%%

program:
	program expr '\n'	{ eval_tree($2); }
	|
	;

expr:
	NUM { $$ = new num_node($1); }
	| IDENTIFIER { $$ = new identifier_node(new std::string($1)); }
	| IDENTIFIER '=' expr { $$ = new assignment_node(new std::string($1), $3); }
	| expr '+' expr { $$ = new binary_operator_node('+', $1, $3); }
	| expr '-' expr { $$ = new binary_operator_node('-', $1, $3); }
	| expr '*' expr { $$ = new binary_operator_node('*', $1, $3);; }
	| expr '/' expr { $$ = new binary_operator_node('/', $1, $3); }
	| '-' expr %prec NEG { $$ = new unary_operator_node('-', $2); }
	| '(' expr ')' { $$ = $2; }
	;

%%

void yyerror(char* s)
{
	fprintf(stderr, "%s\n", s);
}

