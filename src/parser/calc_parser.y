%{
#include <stdio.h>
#include <string>
#include "calc.h"
int c_comp_lex(void);
void c_comp_error(char*);
%}

%define api.prefix c_comp_

%define api.value.type union
%token <double> NUM
%token <char*> IDENTIFIER
%token PRINT
%type <struct tree_node*> expr

%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'

%start program

%%

program:
	statement_list
	| block
	|
	;

block:
	'{' statement_list '}'

statement_list:
	statement_list statement
	|
	;

statement:
	PRINT '(' expr ')' { printf("%.10g\n", solve_node($3)); }
	| expr { solve_node($1); }

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

void c_comp_error(char* s)
{
	fprintf(stderr, "%s\n", s);
}

