#include <functional>

#include "calc.h"

#include "util/variant_list.h"

enum class Tokens
{
    program,
    translation_unit,
    function,
    parameter_list,
    statement_list,
    block,
    statement,
    expr,
	end_of_input
};

template <typename TokenType>
class production
{
	public:

	production<TokenType> execute(std::function<TokenType(VariantList&)> func);

	template <typename ReadType>
	production<TokenType> read();

	template <typename ReadType>
	production<TokenType> read(std::string production_name);

	template <typename ReadType>
	production<TokenType> read(ReadType value);

	typedef TokenType TType;
};

template <typename ParseType>
class grammar
{
	public:

	template <typename TokenType>
	production<TokenType> add_production();

	template <typename TokenType>
	production<TokenType> add_production(std::string production_name);
};

void parse_definition()
{
	grammar<assignment_node> g;

	g.add_production<assignment_node>("Assign")
		.read<std::string>()
		.read('=')
		.read<expr_node>("Sums")
		.execute([](VariantList &v) {
			auto& id = v.get<std::string>(0);
			auto& expr = v.get<expr_node>(2);
			return assignment_node(id, expr);
		});

	g.add_production<expr_node>("Sums")
		.read<expr_node>("Sums")
		.read('+')
		.read<expr_node>("Products")
		.execute([](VariantList &v) {
			auto& sums = v.get<expr_node>(0);
			auto& products = v.get<expr_node>(2);
			return binary_operator_node('+', sums, products);
		});

	g.add_production<expr_node>("Sums")
		.read<expr_node>("Products")
		.execute([](VariantList &v) {
			auto& products = v.get<expr_node>(0);
			return products;
		});

	g.add_production<expr_node>("Products")
		.read<expr_node>("Products")
		.read('*')
		.read<expr_node>("Value")
		.execute([](VariantList &v) {
			auto& products = v.get<expr_node>(0);
			auto& value = v.get<expr_node>(2);
			return binary_operator_node('*', products, value);
		});

	g.add_production<expr_node>("Products")
		.read<expr_node>("Value")
		.execute([](VariantList &v) {
			auto& value = v.get<expr_node>(0);
			return value;
		});
	
	g.add_production<expr_node>("Value")
		.read<double>()
		.execute([](VariantList &v) {
			auto& value = v.get<double>(0);
			return num_node(value);
		});
	
	g.add_production<expr_node>("Value")
		.read<identifier_node>()
		.execute([](VariantList &v) {
			auto& value = v.get<identifier_node>(0);
			return value;
		});
	
	g.add_production<identifier_node>()
		.read<std::string>()
		.execute([](VariantList &v) {
			auto& value = v.get<std::string>(0);
			return identifier_node(value);
		});
}