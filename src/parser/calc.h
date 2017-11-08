#include <string>
#include <map>

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
    NUM,
    BINARY_OPERATOR,
    UNARY_OPERATOR,
    ASSIGNMENT,
    IDENTIFIER
};

struct tree_node
{
    node_type type;
};

struct num_node : public tree_node
{
    num_node(double val)
    {
        type = node_type::NUM;
        value = val;
    }

    double value;
};

struct binary_operator_node : public tree_node
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

struct unary_operator_node : public tree_node
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

struct assignment_node : public tree_node
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

struct identifier_node : public tree_node
{
    identifier_node(std::string* n)
    {
        type = node_type::IDENTIFIER;
        name = n;
    }

    std::string* name;
};

double solve_node(tree_node* node);
