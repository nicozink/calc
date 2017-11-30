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
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

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

    throw "Unknown symbol for binary operator.";
}

double solve_node(unary_operator_node* node)
{
    if (node->symbol == '-')
    {
        return -solve_node(node->expr);
    }

    throw "Unknown symbol for unary operator.";
}

double solve_node(assignment_node* node)
{
    std::string* name = node->left;
    double value = solve_node(node->right);

    calc_symbol* symbol = find_symbol(*name);

    if (symbol != nullptr)
    {
        symbol->value = value;
        return value;
    }
    else
    {
        throw "Variable doesn't exist.";
    }
}

double solve_node(expression_statement_node* node)
{
    solve_node(node->node);
}

double solve_node(variable_declaration_node* node)
{
    std::string* name = node->left;
    double value = solve_node(node->right);

    if (symbols[symbols.size() - 1]->exists(*name))
    {
        throw "Variable already exists.";
    }
    else
    {
        symbols[symbols.size() - 1]->add(*name, value);
        return value;
    }
}

double solve_node(print_node* node)
{
    double value = solve_node(node->node);

    printf("%.10g\n", value);
}

double solve_node(return_node* node)
{
    solve_node(node->node);
}

double solve_node(identifier_node* node)
{
    std::string* name = node->name;

    calc_symbol* symbol = find_symbol(*name);
    if (symbol != nullptr)
    {
        return symbol->value;
    }

    throw "Unknown identifier.";
}

double solve_node(function_call_node* node)
{
    std::string* name = node->name;

    auto it = functions.find(*name);
    if (it != functions.end())
    {
        function_node* function_node = it->second;

        solve_node(function_node->block);
    }
    else
    {
        throw "Unknown function.";
    }
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
    symbols.push_back(new symbol_table());
    solve_node(node->statements);
    symbols.pop_back();
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

        case node_type::EXPRESSION_STATEMENT:
            return solve_node(static_cast<expression_statement_node*>(node));
        
        case node_type::VARIABLE_DECLARATION:
            return solve_node(static_cast<variable_declaration_node*>(node));
        
        case node_type::PRINT:
            return solve_node(static_cast<print_node*>(node));
        
        case node_type::RETURN:
            return solve_node(static_cast<return_node*>(node));
        
        case node_type::FUNCTION_CALL:
            return solve_node(static_cast<function_call_node*>(node));
        
        case node_type::IDENTIFIER:
            return solve_node(static_cast<identifier_node*>(node));
        
        case node_type::STATEMENT_LIST:
            return solve_node(static_cast<statement_list_node*>(node));

        case node_type::BLOCK:
            return solve_node(static_cast<block_node*>(node));
    }

    throw "Unknown token type.";
}

void interpret(translation_unit* unit)
{
    llvm::LLVMContext &context = llvm::getGlobalContext();
    llvm::SMDiagnostic Err;
    
    std::unique_ptr<llvm::Module> module = std::unique_ptr<llvm::Module>(new llvm::Module("asdf", context));
    llvm::IRBuilder<> builder(context);
   
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc = 
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", &*module);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);
   
    llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!");
   
    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
   
    llvm::FunctionType *putsType = 
      llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
    llvm::Constant *putsFunc = module->getOrInsertFunction("puts", putsType);
   
    builder.CreateCall(putsFunc, helloWorld);
    builder.CreateRetVoid();
  
    if(module)
    {
        llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::move(module)).create();
  
        llvm::Function* func = ee->FindFunctionNamed("main");
      
        int r = ee->runFunctionAsMain(func, std::vector< std::string >(), nullptr);
      
        delete ee;
    }

    symbols.push_back(new symbol_table());
    functions = unit->functions;

    function_call_node main_function(new std::string("main"));
    solve_node(&main_function);
}