%{
#include <stdio.h>
#include <string>
#include "calc.h"
#include "interpreter/interpreter.h"
int c_comp_lex(void);
void c_comp_error(char*);
%}

%define api.prefix c_comp_

%define api.value.type union
%token <double> NUM
%token <char*> IDENTIFIER
%token PRINT
%token FUNCTION
%token RETURN
%token VAR

%type <struct translation_unit*> translation_unit
%type <struct function_node*> function
%type <struct parameter_list_node*> parameter_list
%type <struct block_node*> block
%type <struct statement_list_node*> statement_list
%type <struct statement_node*> statement
%type <struct expr_node*> expr

%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'

%start program

%%

program:
	translation_unit {
		Interpreter interp;
		interp.add($1);
		interp.execute();
	}

translation_unit:
	translation_unit function { $$->add($2); }
	| { $$ = new translation_unit(); }
	;

function:
	FUNCTION IDENTIFIER '(' parameter_list ')' block { $$ = new function_node(new std::string($2), $4, $6); }

parameter_list:
	parameter_list IDENTIFIER { $1->add(new std::string($2)); $$ = $1; }
	| { $$ = new parameter_list_node(); }
	;

statement_list:
	statement_list statement { $1->add($2); $$=$1; }
	| { $$ = new statement_list_node(); }
	;

block:
	'{' statement_list '}' { $$ = new block_node($2); }

statement:
	PRINT '(' expr ')' { $$ = new print_node($3); }
	| VAR IDENTIFIER '=' expr { $$ = new variable_declaration_node(new std::string($2), $4); }
	| IDENTIFIER '=' expr { $$ = new assignment_node(new std::string($1), $3); }
	| RETURN expr { $$ = new return_node($2); }
	| block { $$ = $1; }
	| expr { $$ = new expression_statement_node($1); }

expr:
	NUM { $$ = new num_node($1); }
	| IDENTIFIER { $$ = new identifier_node(new std::string($1)); }
	| IDENTIFIER '(' ')' { $$ = new function_call_node(new std::string($1)); }
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

