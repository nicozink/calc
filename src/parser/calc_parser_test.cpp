#include <functional>

#include "calc.h"

#include <cpp_util/types/variant_list.h>

#include <parser/parser.h>

enum class Token
{
    assignment,
    sum,
    product,
    value,
    identifier,
	whitespace
};

void parse_definition()
{
	grammar<Token> g;

	g.add_production(Token::assignment)
		.read_token(Token::identifier)
		.read_value('=')
		.read_token(Token::sum)
		.execute<assignment_node>([](VariantList &v) {
			auto& id = v.get<identifier_node>(0).name;
			auto& expr = v.get<expr_node>(2);
			return assignment_node(id, expr);
		});

	g.add_production(Token::sum)
		.read_token(Token::sum)
		.read_value('+')
		.read_token(Token::product)
		.execute<expr_node>([](VariantList &v) {
			auto& sums = v.get<expr_node>(0);
			auto& products = v.get<expr_node>(2);
			return binary_operator_node('+', sums, products);
		});

	g.add_production(Token::sum)
		.read_token(Token::product)
		.execute<expr_node>([](VariantList &v) {
			auto& products = v.get<expr_node>(0);
			return products;
		});

	g.add_production(Token::product)
		.read_token(Token::product)
		.read_value('*')
		.read_token(Token::value)
		.execute<expr_node>([](VariantList &v) {
			auto& products = v.get<expr_node>(0);
			auto& value = v.get<expr_node>(2);
			return binary_operator_node('*', products, value);
		});

	g.add_production(Token::product)
		.read_token(Token::value)
		.execute<expr_node>([](VariantList &v) {
			auto& value = v.get<expr_node>(0);
			return value;
		});
	
	g.add_production(Token::value)
		.read_type<double>("^(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?$")
		.execute<expr_node>([](VariantList &v) {
			auto& value = v.get<double>(0);
			return num_node(value);
		});
	
	g.add_production(Token::value)
		.read_token(Token::identifier)
		.execute<expr_node>([](VariantList &v) {
			auto& value = v.get<identifier_node>(0);
			return value;
		});
	
	g.add_production(Token::identifier)
		.read_type<std::string>("^[a-zA-Z_$][a-zA-Z_$0-9]*$")
		.execute<identifier_node>([](VariantList &v) {
			auto& value = v.get<std::string>(0);
			return identifier_node(value);
		});
	
	g.add_production(Token::whitespace)
		.read_type<void>("\t\n\r");
}