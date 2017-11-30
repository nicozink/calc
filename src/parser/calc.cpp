#include "calc.h"

void translation_unit::add(function_node* function)
{
    std::string name = *(function->name);

    functions.insert(std::pair<std::string, function_node*>(name, function));
}
