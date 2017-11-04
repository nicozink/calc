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
