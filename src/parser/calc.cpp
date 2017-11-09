#include "calc.h"

symbol_table::symbol_table()
{
    
}

calc_symbol* symbol_table::add(std::string name, double value)
{
    calc_symbol* symbol = new calc_symbol();
    symbol->name = name;
    symbol->value = value;

    symbols.insert(std::pair<std::string, calc_symbol*>(name, symbol));

    return symbol;
}

void symbol_table::clear()
{
    for (auto symbol : symbols)
    {
        delete symbol.second;
    }

    symbols.clear();
}

bool symbol_table::exists(std::string name)
{
    auto it = symbols.find(name);
    return (it != symbols.end());
}

calc_symbol* symbol_table::get(std::string name)
{
    auto it = symbols.find(name);
    if (it != symbols.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

static symbol_table symbols;

double solve_node(num_node* node);
double solve_node(binary_operator_node* node);
double solve_node(unary_operator_node* node);
double solve_node(assignment_node* node);
double solve_node(identifier_node* node);
double solve_node(tree_node* node);

double solve_node(num_node* node)
{
    return node->value;
}

double solve_node(binary_operator_node* node)
{
    double left = solve_node(node->left);
    double right = solve_node(node->right);

    if (node->symbol == '+')
    {
        return left + right;
    }
    else if (node->symbol == '-')
    {
        return left - right;
    }
    else if (node->symbol == '*')
    {
        return left * right;
    }
    else if (node->symbol == '/')
    {
        return left / right;
    }

    throw "Should not be here.";
}

double solve_node(unary_operator_node* node)
{
    if (node->symbol == '-')
    {
        return -solve_node(node->expr);
    }

    throw "Should not be here.";
}

double solve_node(assignment_node* node)
{
    std::string* name = node->left;
    double value = solve_node(node->right);

    if (symbols.exists(*name))
    {
        calc_symbol* symbol = symbols.get(*name);
        symbol->value = value;
        return value;
    }
    else
    {
        symbols.add(*name, value);
        return value;
    }
}

double solve_node(print_node* node)
{
    double value = solve_node(node->node);

    printf("%.10g\n", value);
}

double solve_node(identifier_node* node)
{
    std::string* name = node->name;

    if (symbols.exists(*name))
    {
        calc_symbol* symbol = symbols.get(*name);
        return symbol->value;
    }

    throw "Should not be here.";
}

double solve_node(statement_list_node* node)
{
    for (auto statement : node->statements)
    {
        solve_node(statement);
    }
}

double solve_node(block_node* node)
{
    solve_node(node->statements);
}

double solve_node(tree_node* node)
{
    switch (node->type)
    {
        case node_type::NUM:
            return solve_node(static_cast<num_node*>(node));
        
        case node_type::BINARY_OPERATOR:
            return solve_node(static_cast<binary_operator_node*>(node));

        case node_type::UNARY_OPERATOR:
            return solve_node(static_cast<unary_operator_node*>(node));

        case node_type::ASSIGNMENT:
            return solve_node(static_cast<assignment_node*>(node));

        case node_type::PRINT:
            return solve_node(static_cast<print_node*>(node));
        
        case node_type::IDENTIFIER:
            return solve_node(static_cast<identifier_node*>(node));
        
        case node_type::STATEMENT_LIST:
            return solve_node(static_cast<statement_list_node*>(node));

        case node_type::BLOCK:
            return solve_node(static_cast<block_node*>(node));
    }
}
