%language "c++"
%defines
%define api.prefix CalcParser
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
#include "parser/calc.h"
#include "parser/calc_lexer.h"

// Not sure why this is needed
#define YY_NULLPTR nullptr
}

%code
{
#include <stdio.h>
#include <iostream>
#include <string>
#include "calc_lexer.h"
#include "interpreter/interpreter.h"
void CalcParser_error(char*);
static CalcParser::parser::symbol_type CalcParserlex (CalcLexer& calc_lexer);
}

%parse-param {CalcLexer& calc_lexer}
%lex-param {CalcLexer& calc_lexer}

%token <double> NUM
%token <std::string> IDENTIFIER
%token PRINT
%token FUNCTION
%token RETURN
%token VAR
%token ROUND_BRACKET_OPEN
%token ROUND_BRACKET_CLOSE
%token CURLY_BRACKET_OPEN
%token CURLY_BRACKET_CLOSE
%token EQUALS
%token PLUS
%token MINUS
%token MULTIPLY
%token DIVIDE
%token END_OF_FILE

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
	translation_unit END_OF_FILE {
		Interpreter interp;
		interp.add($1);
		interp.execute();
		YYACCEPT;
	}

translation_unit:
	translation_unit function { $1->add($2); $$=$1; }
	| { $$ = new translation_unit(); }
	;

function:
	FUNCTION IDENTIFIER ROUND_BRACKET_OPEN parameter_list ROUND_BRACKET_CLOSE block { $$ = new function_node($2, $4, $6); }

parameter_list:
	parameter_list IDENTIFIER { $1->add(new std::string($2)); $$ = $1; }
	| { $$ = new parameter_list_node(); }
	;

statement_list:
	statement_list statement { $1->add($2); $$=$1; }
	| { $$ = new statement_list_node(); }
	;

block:
	CURLY_BRACKET_OPEN statement_list CURLY_BRACKET_CLOSE { $$ = new block_node($2); }

statement:
	PRINT ROUND_BRACKET_OPEN expr ROUND_BRACKET_CLOSE { $$ = new print_node($3); }
	| VAR IDENTIFIER EQUALS expr { $$ = new variable_declaration_node(new std::string($2), $4); }
	| IDENTIFIER EQUALS expr { $$ = new assignment_node(new std::string($1), $3); }
	| RETURN expr { $$ = new return_node($2); }
	| block { $$ = $1; }
	| expr { $$ = new expression_statement_node($1); }

expr:
	NUM { $$ = new num_node($1); }
	| IDENTIFIER { $$ = new identifier_node(new std::string($1)); }
	| IDENTIFIER ROUND_BRACKET_OPEN ROUND_BRACKET_CLOSE { $$ = new function_call_node(new std::string($1)); }
	| expr PLUS expr { $$ = new binary_operator_node('+', $1, $3); }
	| expr MINUS expr { $$ = new binary_operator_node('-', $1, $3); }
	| expr MULTIPLY expr { $$ = new binary_operator_node('*', $1, $3);; }
	| expr DIVIDE expr { $$ = new binary_operator_node('/', $1, $3); }
	| MINUS expr %prec NEG { $$ = new unary_operator_node('-', $2); }
	| ROUND_BRACKET_OPEN expr ROUND_BRACKET_CLOSE { $$ = $2; }
	;

%%

void CalcParser::parser::error(const std::string& msg)
{
	std::cerr << msg << std::endl;
}

static CalcParser::parser::symbol_type CalcParserlex(CalcLexer& calc_lexer)
{
	auto token = calc_lexer.read_next();

	std::cout << "Token: " << calc_lexer.get_string() << std::endl;

	switch (token)
	{
		case CalcToken::id_print:
		{
			return CalcParser::parser::make_PRINT();
		}
		case CalcToken::id_function:
		{
			return CalcParser::parser::make_FUNCTION();
		}
		case CalcToken::id_return:
		{
			return CalcParser::parser::make_RETURN();
		}
		case CalcToken::id_var:
		{
			return CalcParser::parser::make_VAR();
		}
		case CalcToken::number:
		{
			return CalcParser::parser::make_NUM(calc_lexer.get_number());
		}
		case CalcToken::identifier:
		{
			return CalcParser::parser::make_IDENTIFIER(calc_lexer.get_string().c_str());
		}
		case CalcToken::round_bracket_open:
		{
			return CalcParser::parser::make_ROUND_BRACKET_OPEN();
		}
		case CalcToken::round_bracket_close:
		{
			return CalcParser::parser::make_ROUND_BRACKET_CLOSE();
		}
		case CalcToken::curly_bracket_open:
		{
			return CalcParser::parser::make_CURLY_BRACKET_OPEN();
		}
		case CalcToken::curly_bracket_close:
		{
			return CalcParser::parser::make_CURLY_BRACKET_CLOSE();
		}
		case CalcToken::equals:
		{
			return CalcParser::parser::make_EQUALS();
		}
		case CalcToken::plus:
		{
			return CalcParser::parser::make_PLUS();
		}
		case CalcToken::minus:
		{
			return CalcParser::parser::make_MINUS();
		}
		case CalcToken::multiply:
		{
			return CalcParser::parser::make_MULTIPLY();
		}
		case CalcToken::divide:
		{
			return CalcParser::parser::make_DIVIDE();
		}
		case CalcToken::end_of_input:
		{
			return CalcParser::parser::make_END_OF_FILE();
		}
	}

	throw "Unknown token.";
}
