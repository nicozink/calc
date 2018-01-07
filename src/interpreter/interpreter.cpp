#include "interpreter.h"

#include <string>
#include <map>
#include <vector>

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

class Interpreter::impl
{
public:

	impl();

	void add(translation_unit* unit);
	void execute();

private:

	void process_assignment(assignment_node& node, llvm::Module* module);
	llvm::Value* process_binary_operator(binary_operator_node& node, llvm::Module* module);
	void process_block(block_node& block, llvm::Module* module);
	llvm::Value* process_expr(expr_node& node, llvm::Module* module);
	void process_expression_statement(expression_statement_node& node, llvm::Module* module);
	void process_function(function_node& function, llvm::Module* module);
	void process_function_block(block_node& block, llvm::Function *mainFunc, llvm::Module* module);
	llvm::Value* process_function_call(function_call_node& node, llvm::Module* module);
	llvm::Value* process_identifier(identifier_node& node, llvm::Module* module);
	llvm::Value* process_num(num_node& node, llvm::Module* module);
	void process_print(print_node& node, llvm::Module* module);
	void process_return(return_node& node, llvm::Module* module);
	void process_statement(statement_node& node, llvm::Module* module);
	void process_statement_list(statement_list_node& node, llvm::Module* module);
	llvm::Value* process_unary_operator(unary_operator_node& node, llvm::Module* module);
	void process_variable_declaration(variable_declaration_node& node, llvm::Module* module);

	llvm::LLVMContext& context;
	llvm::ExecutionEngine *ee;
	llvm::IRBuilder<> builder;
};

Interpreter::impl::impl()
: context{ llvm::getGlobalContext() },
  builder{ llvm::IRBuilder<>(context) }
{

}

void Interpreter::impl::add(translation_unit* unit)
{
	llvm::Module* module = new llvm::Module("translation_unit", context);

	for (auto function : unit->functions)
	{
		process_function(*function.second, module);
	}

	module->dump();

	ee = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).create();
}

void Interpreter::impl::execute()
{
	llvm::Function* func = ee->FindFunctionNamed("main");

	int r = ee->runFunctionAsMain(func, std::vector< std::string >(), nullptr);

	delete ee;
}

struct calc_symbol
{
	std::string name;
	llvm::AllocaInst* value;
};

class symbol_table
{
public:

	symbol_table();

	calc_symbol* add(std::string name, llvm::AllocaInst* value);

	void clear();

	bool exists(std::string name);

	calc_symbol* get(std::string name);

private:

	std::map<std::string, calc_symbol*> symbols;
};

std::vector<symbol_table*> symbols;

std::map<std::string, function_node*> functions;

calc_symbol* find_symbol(std::string name)
{
	for (auto it = symbols.rbegin(); it != symbols.rend(); it++)
	{
		symbol_table* symbol_table = *it;
		if (symbol_table->exists(name))
		{
			return symbol_table->get(name);
		}
	}

	return nullptr;
}

symbol_table::symbol_table()
{

}

calc_symbol* symbol_table::add(std::string name, llvm::AllocaInst* value)
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

llvm::Value* Interpreter::impl::process_num(num_node& node, llvm::Module* module)
{
	llvm::Value* constValue = llvm::ConstantFP::get(context, llvm::APFloat(node.value));

	return constValue;
}

llvm::Value* Interpreter::impl::process_binary_operator(binary_operator_node& node, llvm::Module* module)
{
	llvm::Value* left = process_expr(*node.left, module);
	llvm::Value* right = process_expr(*node.right, module);

	llvm::Value* result;

	if (node.symbol == '+')
	{
		result = builder.CreateFAdd(left, right);
	}
	else if (node.symbol == '-')
	{
		result = builder.CreateFSub(left, right, "tmp");
	}
	else if (node.symbol == '*')
	{
		result = builder.CreateFMul(left, right);
	}
	else if (node.symbol == '/')
	{
		result = builder.CreateFDiv(left, right);
	}
	else
	{
		throw "Unknown symbol for binary operator.";
	}

	return result;
}

llvm::Value* Interpreter::impl::process_unary_operator(unary_operator_node& node, llvm::Module* module)
{
	llvm::Value* value = process_expr(*node.expr, module);

	llvm::Value* result;

	if (node.symbol == '-')
	{
		result = builder.CreateFNeg(value);
	}
	else
	{
		throw "Unknown symbol for binary operator.";
	}

	return result;
}

llvm::Value* Interpreter::impl::process_function_call(function_call_node& node, llvm::Module* module)
{
	llvm::FunctionType *function_type = llvm::TypeBuilder<void(), false>::get(context);

	llvm::Constant* function = module->getOrInsertFunction(*node.name, function_type, llvm::AttributeSet().addAttribute(module->getContext(), 1U, llvm::Attribute::NoAlias));

	std::vector<llvm::Value*> printArgs;
	llvm::ArrayRef<llvm::Value*>  argsRef(printArgs);

	builder.CreateCall(function, argsRef);

	return nullptr;
}

llvm::Value* Interpreter::impl::process_identifier(identifier_node& node, llvm::Module* module)
{
	std::string* name = node.name;

	calc_symbol* symbol = find_symbol(*name);
	if (symbol != nullptr)
	{
		llvm::AllocaInst* alloca = symbol->value;
		return builder.CreateLoad(alloca);
	}

	throw "Unknown identifier.";
}

llvm::Value* Interpreter::impl::process_expr(expr_node& node, llvm::Module* module)
{
	switch (node.type)
{
		case node_type::NUM:
			return process_num(static_cast<num_node&>(node), module);

		case node_type::BINARY_OPERATOR:
			return process_binary_operator(static_cast<binary_operator_node&>(node), module);

		case node_type::UNARY_OPERATOR:
			return process_unary_operator(static_cast<unary_operator_node&>(node), module);

		case node_type::FUNCTION_CALL:
			return process_function_call(static_cast<function_call_node&>(node), module);
	
		case node_type::IDENTIFIER:
			return process_identifier(static_cast<identifier_node&>(node), module);
	}

	throw "Unknown token type.";
}

void Interpreter::impl::process_assignment(assignment_node& node, llvm::Module* module)
{
	std::string* name = node.name;

	calc_symbol* symbol = find_symbol(*name);
	if (symbol != nullptr)
	{
		llvm::AllocaInst* alloca = symbol->value;

		llvm::Value* value = process_expr(*node.expr, module);

		builder.CreateStore(value, alloca);

		return;
	}

	throw "Unknown identifier.";
}

void Interpreter::impl::process_variable_declaration(variable_declaration_node& node, llvm::Module* module)
{
	std::string* name = node.name;

	if (symbols[symbols.size() - 1]->exists(*name))
	{
		throw "Variable already exists.";
	}

	llvm::AllocaInst* alloca = builder.CreateAlloca(llvm::Type::getDoubleTy(context), 0, node.name->c_str());

	llvm::Value* value = process_expr(*node.expr, module);

	builder.CreateStore(value, alloca);

	symbols[symbols.size() - 1]->add(*name, alloca);
}

void Interpreter::impl::process_print(print_node& node, llvm::Module* module)
{
	llvm::Value *expr = process_expr(*node.expr, module);

	llvm::FunctionType *printf_type = llvm::TypeBuilder<int(char *, ...), false>::get(context);

	llvm::Constant* printf_func = module->getOrInsertFunction("printf", printf_type, llvm::AttributeSet().addAttribute(module->getContext(), 1U, llvm::Attribute::NoAlias));

	std::vector<llvm::Value*> printArgs;
	printArgs.push_back(builder.CreateGlobalStringPtr("out: %f\n"));
	printArgs.push_back(expr);
	llvm::ArrayRef<llvm::Value*> argsRef(printArgs);

	builder.CreateCall(printf_func, argsRef);
}

void Interpreter::impl::process_return(return_node& node, llvm::Module* module)
{
	builder.CreateRetVoid();
}

void Interpreter::impl::process_expression_statement(expression_statement_node& node, llvm::Module* module)
{
	process_expr(*node.expr, module);
}

void Interpreter::impl::process_block(block_node& block, llvm::Module* module)
{
	symbols.push_back(new symbol_table());

	process_statement_list(*block.statements, module);

	symbols.pop_back();
}

void Interpreter::impl::process_statement(statement_node& node, llvm::Module* module)
{
	switch (node.type)
	{
		case node_type::ASSIGNMENT:
			process_assignment(static_cast<assignment_node&>(node), module);
			return;

		case node_type::EXPRESSION_STATEMENT:
			process_expression_statement(static_cast<expression_statement_node&>(node), module);
			return;
		
		case node_type::VARIABLE_DECLARATION:
			process_variable_declaration(static_cast<variable_declaration_node&>(node), module);
			return;
		
		case node_type::PRINT:
			process_print(static_cast<print_node&>(node), module);
			return;
		
		case node_type::RETURN:
			process_return(static_cast<return_node&>(node), module);
			return;
		
		case node_type::BLOCK:
			process_block(static_cast<block_node&>(node), module);
			return;
	}

	throw "Unknown token type.";
}

void Interpreter::impl::process_statement_list(statement_list_node& node, llvm::Module* module)
{
	for (auto statement : node.statements)
	{
		process_statement(*statement, module);
	}
}

void Interpreter::impl::process_function_block(block_node& block, llvm::Function *mainFunc, llvm::Module* module)
{
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
	builder.SetInsertPoint(entry);
   
	process_statement_list(*block.statements, module);

	builder.CreateRetVoid();
}

void Interpreter::impl::process_function(function_node& function, llvm::Module* module)
{
	symbols.push_back(new symbol_table());

	llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
	llvm::Function *mainFunc = 
	  llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, function.name->c_str(), &*module);

	process_function_block(*function.block, mainFunc, module);

	symbols.pop_back();
}

Interpreter::Interpreter()
: p_impl(new Interpreter::impl)
{

}

Interpreter::~Interpreter() = default;

void Interpreter::add(translation_unit* unit)
{
	p_impl->add(unit);
}

void Interpreter::execute()
{
	p_impl->execute();
}
