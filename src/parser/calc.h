#include <string>
#include <map>
#include <vector>

struct calc_symbol
{
    std::string name;
    double value;
};

class symbol_table
{
public:

    symbol_table();

    calc_symbol* add(std::string name, double value);

    void clear();

    bool exists(std::string name);

    calc_symbol* get(std::string name);

private:

    std::map<std::string, calc_symbol*> symbols;
};

enum class node_type
{
    BLOCK,
    STATEMENT_LIST,
    NUM,
    BINARY_OPERATOR,
    UNARY_OPERATOR,
    ASSIGNMENT,
    PRINT,
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
    binary_operator_node(char s, tree_node* l, tree_node* r)
    {
        type = node_type::BINARY_OPERATOR;
        symbol = s;
        left = l;
        right = r;
    }

    char symbol;
    tree_node* left;
    tree_node* right;
};

struct unary_operator_node : public expr_node
{
    unary_operator_node(char s, tree_node* e)
    {
        type = node_type::UNARY_OPERATOR;
        symbol = s;
        expr = e;
    }

    char symbol;
    tree_node* expr;
};

struct statement_node : public tree_node
{

};

struct assignment_node : public statement_node
{
    assignment_node(std::string* l, tree_node* r)
    {
        type = node_type::ASSIGNMENT;
        left = l;
        right = r;
    }

    std::string* left;
    tree_node* right;
};

struct print_node : public statement_node
{
    print_node(tree_node* n)
    {
        type = node_type::PRINT;
        node = n;
    }

    tree_node* node;
};

struct identifier_node : public expr_node
{
    identifier_node(std::string* n)
    {
        type = node_type::IDENTIFIER;
        name = n;
    }

    std::string* name;
};

struct statement_list_node : public tree_node
{
    statement_list_node()
    {
        type = node_type::STATEMENT_LIST;
    }

    void add(tree_node* statement)
    {
        statements.push_back(statement);
    }

    std::vector<tree_node*> statements;
};

struct block_node : public tree_node
{
    block_node(statement_list_node* s)
    {
        type = node_type::BLOCK;
        statements = s;
    }

    statement_list_node* statements;
};

double solve_node(tree_node* node);
