#pragma once

#include <string>
#include <map>
#include <vector>

enum class node_type
{
    FUNCTION,
    FUNCTION_CALL,
    PARAMETER_LIST,
    BLOCK,
    STATEMENT_LIST,
    NUM,
    BINARY_OPERATOR,
    UNARY_OPERATOR,
    ASSIGNMENT,
    EXPRESSION_STATEMENT,
    VARIABLE_DECLARATION,
    PRINT,
    RETURN,
    IDENTIFIER
};

struct tree_node
{
    node_type type;
};

struct expr_node : public tree_node
{

};

struct num_node : public expr_node
{
    num_node(double val)
    {
        type = node_type::NUM;
        value = val;
    }

    double value;
};

struct binary_operator_node : public expr_node
{
    binary_operator_node(char s, expr_node l, expr_node r)
    {
        type = node_type::BINARY_OPERATOR;
        symbol = s;
        left = l;
        right = r;
    }

    char symbol;
    expr_node left;
    expr_node right;
};

struct unary_operator_node : public expr_node
{
    unary_operator_node(char s, expr_node* e)
    {
        type = node_type::UNARY_OPERATOR;
        symbol = s;
        expr = e;
    }

    char symbol;
    expr_node* expr;
};

struct statement_node : public tree_node
{

};

struct assignment_node : public statement_node
{
    assignment_node(std::string n, expr_node e)
    {
        type = node_type::ASSIGNMENT;
        name = n;
        expr = e;
    }

    std::string name;
    expr_node expr;
};

struct variable_declaration_node : public statement_node
{
    variable_declaration_node(std::string* n, expr_node* e)
    {
        type = node_type::VARIABLE_DECLARATION;
        name = n;
        expr = e;
    }

    std::string* name;
    expr_node* expr;
};

struct print_node : public statement_node
{
    print_node(expr_node* e)
    {
        type = node_type::PRINT;
        expr = e;
    }

    expr_node* expr;
};

struct return_node : public statement_node
{
    return_node(tree_node* n)
    {
        type = node_type::RETURN;
        node = n;
    }

    tree_node* node;
};

struct expression_statement_node : public statement_node
{
    expression_statement_node(expr_node* e)
    {
        type = node_type::EXPRESSION_STATEMENT;
        expr = e;
    }

    expr_node* expr;
};

struct function_call_node : public expr_node
{
    function_call_node(std::string* n)
    {
        type = node_type::FUNCTION_CALL;
        name = n;
    }

    std::string* name;
};

struct identifier_node : public expr_node
{
    identifier_node(std::string n)
    {
        type = node_type::IDENTIFIER;
        name = n;
    }

    std::string name;
};

struct statement_list_node : public tree_node
{
    statement_list_node()
    {
        type = node_type::STATEMENT_LIST;
    }

    void add(statement_node* statement)
    {
        statements.push_back(statement);
    }

    std::vector<statement_node*> statements;
};

struct block_node : public statement_node
{
    block_node(statement_list_node* s)
    {
        type = node_type::BLOCK;
        statements = s;
    }

    statement_list_node* statements;
};

struct parameter_list_node : public tree_node
{
    parameter_list_node()
    {
        type = node_type::PARAMETER_LIST;
    }

    void add(std::string* parameter)
    {
        parameters.push_back(parameter);
    }

    std::vector<std::string*> parameters;
};

struct function_node : public tree_node
{
    function_node(std::string n, parameter_list_node p, block_node b)
    : name{ n }, block{ b }, parameters{ p }
    {
        type = node_type::FUNCTION;
    }

    std::string name; 
    parameter_list_node parameters;
    block_node block;
};

struct translation_unit
{
    translation_unit()
    {
        
    }

    void add(function_node function);

    std::map<std::string, function_node> functions;
};
