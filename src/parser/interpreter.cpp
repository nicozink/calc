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

llvm::Value* process_expr(expr_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module);

llvm::Value* process_num(num_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    llvm::Value* constValue = llvm::ConstantFP::get(context, llvm::APFloat(node.value));

    return constValue;
}

llvm::Value* process_binary_operator(binary_operator_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    llvm::Value* left = process_expr(*node.left, context, builder, module);
    llvm::Value* right = process_expr(*node.right, context, builder, module);

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

llvm::Value* process_unary_operator(unary_operator_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    return nullptr;
}

llvm::Value* process_function_call(function_call_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    llvm::FunctionType *function_type = llvm::TypeBuilder<void(), false>::get(context);

    llvm::Constant* function = module->getOrInsertFunction(*node.name, function_type, llvm::AttributeSet().addAttribute(module->getContext(), 1U, llvm::Attribute::NoAlias));

    std::vector<llvm::Value*> printArgs;
    llvm::ArrayRef<llvm::Value*>  argsRef(printArgs);

    builder.CreateCall(function, argsRef);

    return nullptr;
}

llvm::Value* process_identifier(identifier_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    return nullptr;
}

llvm::Value* process_expr(expr_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    switch (node.type)
    {
        case node_type::NUM:
            return process_num(static_cast<num_node&>(node), context, builder, module);
        
        case node_type::BINARY_OPERATOR:
            return process_binary_operator(static_cast<binary_operator_node&>(node), context, builder, module);

        case node_type::UNARY_OPERATOR:
            return process_unary_operator(static_cast<unary_operator_node&>(node), context, builder, module);

        case node_type::FUNCTION_CALL:
            return process_function_call(static_cast<function_call_node&>(node), context, builder, module);
        
        case node_type::IDENTIFIER:
            return process_identifier(static_cast<identifier_node&>(node), context, builder, module);
    }

    throw "Unknown token type.";
}

void process_assignment(assignment_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{

}

void process_variable_declaration(variable_declaration_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{

}

void process_print(print_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    llvm::Value *expr = process_expr(*node.expr, context, builder, module);
    
    llvm::FunctionType *printf_type = llvm::TypeBuilder<int(char *, ...), false>::get(context);

    llvm::Constant* printf_func = module->getOrInsertFunction("printf", printf_type, llvm::AttributeSet().addAttribute(module->getContext(), 1U, llvm::Attribute::NoAlias));

    std::vector<llvm::Value*> printArgs;
    printArgs.push_back(builder.CreateGlobalStringPtr("out: %f\n"));
    printArgs.push_back(expr);
    llvm::ArrayRef<llvm::Value*> argsRef(printArgs);

    builder.CreateCall(printf_func, argsRef);
}

void process_return(return_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    builder.CreateRetVoid();
}

void process_expression_statement(expression_statement_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    process_expr(*node.expr, context, builder, module);
}

void process_statement(statement_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    switch (node.type)
    {
        case node_type::ASSIGNMENT:
            return process_assignment(static_cast<assignment_node&>(node), context, builder, module);

        case node_type::EXPRESSION_STATEMENT:
            return process_expression_statement(static_cast<expression_statement_node&>(node), context, builder, module);
        
        case node_type::VARIABLE_DECLARATION:
            return process_variable_declaration(static_cast<variable_declaration_node&>(node), context, builder, module);
        
        case node_type::PRINT:
            return process_print(static_cast<print_node&>(node), context, builder, module);
        
        case node_type::RETURN:
            return process_return(static_cast<return_node&>(node), context, builder, module);
        
        //case node_type::BLOCK:
        //    return solve_node(static_cast<block_node*>(node));
    }

    throw "Unknown token type.";
}

void process_statement_list(statement_list_node& node, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    for (auto statement : node.statements)
    {
        process_statement(*statement, context, builder, module);
    }
}

void process_block(block_node& block, llvm::Function *mainFunc, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);
   
    process_statement_list(*block.statements, context, builder, module);

    builder.CreateRetVoid();
}

void process_function(function_node& function, llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Module* module)
{
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc = 
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, function.name->c_str(), &*module);
    
    process_block(*function.block, mainFunc, context, builder, module);
}

void interpret(translation_unit* unit)
{
    llvm::LLVMContext &context = llvm::getGlobalContext();
    llvm::SMDiagnostic Err;
    
    llvm::Module* module = new llvm::Module("translation_unit", context);
    llvm::IRBuilder<> builder(context);
  
    for (auto function : unit->functions)
    {
        process_function(*function.second, context, builder, module);
    }

    module->dump();

    llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).create();
    
    llvm::Function* func = ee->FindFunctionNamed("main");
        
    int r = ee->runFunctionAsMain(func, std::vector< std::string >(), nullptr);
        
    delete ee;
}