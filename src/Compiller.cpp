#include "Compiller.h"
#include "IRGenerator.hpp"
#include "termcolor/termcolor.hpp"
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Intrinsics.h>

void TranslateToAsm(std::vector<DifferTree*>& proga, const char* out_name)
{
    TranslateProcessing(proga);
}


void DeclareStdlib(IRGenerator& gen) {
    auto* voidType = llvm::Type::getVoidTy(gen.context);
    auto* intType = llvm::Type::getInt32Ty(gen.context);
    
    auto* simFlushType = llvm::FunctionType::get(voidType, false);
    llvm::Function::Create(simFlushType, llvm::Function::ExternalLinkage, "simFlush", gen.module);

    auto* simInitType = llvm::FunctionType::get(voidType, false);
    llvm::Function::Create(simFlushType, llvm::Function::ExternalLinkage, "simInit", gen.module);

    auto* simExitType = llvm::FunctionType::get(voidType, false);
    llvm::Function::Create(simFlushType, llvm::Function::ExternalLinkage, "simExit", gen.module);

    auto* simClearWindowType = llvm::FunctionType::get(voidType, false);
    llvm::Function::Create(simFlushType, llvm::Function::ExternalLinkage, "simClearWindow", gen.module);

    auto* simPutPixelType = llvm::FunctionType::get(voidType, {intType, intType, intType}, false);
    llvm::Function::Create(simPutPixelType, llvm::Function::ExternalLinkage, "simPutPixel", gen.module);
}

void TranslateProcessing(std::vector<DifferTree*>& proga)
{
    auto&& functions = CreateLstFuncNode(proga);

    IRGenerator generator;
    DeclareStdlib(generator);
    for (auto&& functionTree : proga) {
        TreeTranslate(functionTree, functions, generator);
    }
    generator.dump();
}

std::vector<node_t*> CreateLstFuncNode(std::vector<DifferTree*>& proga)
{
    std::vector<node_t*> res;
    for (auto&& functionTree : proga) {
        res.push_back(functionTree->Root()->GetRight());
    }
    return res;
}


void TreeTranslate(DifferTree* function, const std::vector<node_t*>& functions, IRGenerator& gen)
{
    node_t* func = function->Root();

    func = func->GetRight();
    auto* llvm_function = WriteFuncProlog(func, gen);
    gen.current_function = llvm_function;

    TranslateOpSequence(gen, functions, func->GetRight());

    WriteFuncEpilog(llvm_function, gen);
}

auto GetArgumentInfo(IRGenerator& gen, node_t* node)
{
    std::vector<llvm::Type*> variables;
    std::vector<std::string> names;

    if (node->GetLeft())
    {
        node = node->GetLeft();
        while (node->dType() == DataType::COMMA)
        {
            auto* var_ptr = node->GetRight();
            if (var_ptr->dType() == DataType::NEW_VAR) {
                variables.push_back(gen.builder.getDoubleTy());
                names.push_back(var_ptr->Name());
            }
            else
            {
                variables.push_back(gen.builder.getDoubleTy()->getPointerTo());
                names.push_back(var_ptr->Name());
            }
        }
        if (node->dType() == DataType::NEW_VAR) {
            variables.push_back(gen.builder.getDoubleTy());
            names.push_back(node->Name());
        }
        else
        {
            variables.push_back(gen.builder.getDoubleTy()->getPointerTo());
            names.push_back(node->Name());
        }
    }

    return std::make_tuple(variables, names);
}

llvm::Function* WriteFuncProlog(node_t* func, IRGenerator& gen) {
    using namespace llvm;

    auto funcName = func->Name();
    auto [argTypes, argNames] = GetArgumentInfo(gen, func);
    auto* funcType = FunctionType::get(Type::getDoubleTy(gen.context), argTypes, false);


    auto* llvmFunc = gen.module.getFunction(funcName);
    if (!llvmFunc) {
        llvmFunc = Function::Create(funcType, Function::ExternalLinkage, funcName, &gen.module);
    }

    unsigned idx = 0;
    for (auto&& arg : llvmFunc->args()) {
        arg.setName(argNames[idx]);
        ++idx;
    }

    auto* entryBlock = BasicBlock::Create(gen.context, "entry", llvmFunc);
    
    gen.builder.SetInsertPoint(entryBlock);

    // Copy arguments to stack
    for (auto&& arg : llvmFunc->args()) {
        auto localVarName = arg.getName().str() + "_local";
        auto* localVar = gen.builder.CreateAlloca(arg.getType(), nullptr, localVarName);
        if (arg.getType()->isPointerTy()) {
            auto* loadedValue = gen.builder.CreateLoad(Type::getDoubleTy(gen.context), &arg, arg.getName() + "_loaded");
            gen.builder.CreateStore(loadedValue, localVar);
        } else {
            gen.builder.CreateStore(&arg, localVar);
        }
        gen.local_vars[arg.getName().str()] = localVar;
    }

    return llvmFunc;
}

void WriteFuncEpilog(llvm::Function* llvmFunc, IRGenerator& gen) {
    using namespace llvm;
    
    // Copy updated local variables back to their original locations
    for (auto&& arg : llvmFunc->args()) {
        if (arg.getType()->isPointerTy()) {
            auto* updatedValuePtr = gen.local_vars[arg.getName().str()];
            auto* updatedValue = gen.builder.CreateLoad(Type::getDoubleTy(gen.context), updatedValuePtr);
            gen.builder.CreateStore(updatedValue, &arg);
        }
    }

    if (!gen.builder.GetInsertBlock()->getTerminator()) {
        auto zero = CreateFloatConstant(gen, 0.0);
        gen.builder.CreateRet(zero);
    }
}


void TranslateOpSequence(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node)
{
    while (node && (node->dType() == DataType::END_OP))
    {
        TranslateOp(gen, functions, node->GetLeft());
        node = node->GetRight();
    }
}

void TranslateOp(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node)
{   
    switch (node->dType())
    {
    case DataType::FUNC:
        TranslateCallFunc(gen, functions, node);
        break;
    case DataType::PRINTF:
        TranslateCallPrintf(gen, functions, node);
        break;
    case DataType::SCANF:
        TranslateCallInput(gen, functions, node);
        break;
    case DataType::SQRT:
        TranslateCallSqrt(gen, functions, node);
        break;
    case DataType::LOG:
        TranslateCallLog(gen, functions, node);
        break;
    case DataType::SIN:
        TranslateCallSin(gen, functions, node);
        break;
    case DataType::COS:
        TranslateCallCos(gen, functions, node);
        break;
    case DataType::INITIALIZATE:
        TranslateInit(gen, functions, node);
        break;
    case DataType::MOV:
        TranslateMov(gen, functions, node);
        break;
    case DataType::IF:
        TranslateIf(gen, functions, node);
        break;
    case DataType::WHILE:
        TranslateWhile(gen, functions, node);
        break;
    case DataType::RET:
        TranslateRet(gen, functions, node);
        break;
    default:
        break;
    }
}

llvm::Function* GetOrCreateFunction(IRGenerator& gen, node_t* func) {
    using namespace llvm;

    std::string funcName = func->Name();

    Function* targetFunc = gen.module.getFunction(funcName);
    if (!targetFunc) {
        auto [argTypes, argNames] = GetArgumentInfo(gen, func);
        auto* funcType = FunctionType::get(Type::getDoubleTy(gen.context), argTypes, false);
        targetFunc = Function::Create(funcType, Function::ExternalLinkage, funcName, &gen.module);
    }

    return targetFunc;
}

bool IsStdlibFunction(node_t* node) {
    std::string functionName = node->Name();
    return functionName == "simPutPixel" ||
           functionName == "simClearWindow" ||
           functionName == "simFlush" ||
           functionName == "simInit" ||
           functionName == "simExit";
}

llvm::Value* TranslateCallStdlib(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    std::string functionName = node->Name();
    auto* llvmFunction = gen.module.getFunction(functionName);
    
    // Collect parameters
    std::vector<llvm::Value*> args;
    node = node->GetRight();
    if (node) {
        while (node->dType() == DataType::COMMA) {
            auto* arg = TranslateExp(gen, functions, node->GetRight());
            args.push_back(gen.builder.CreateFPToSI(arg, gen.builder.getInt32Ty()));
            node = node->GetLeft();
        }
        auto* arg = TranslateExp(gen, functions, node);
        args.push_back(gen.builder.CreateFPToSI(arg, gen.builder.getInt32Ty()));
    }

    std::reverse(args.begin(), args.end());
    return gen.builder.CreateCall(llvmFunction, args);
}

llvm::Value* TranslateCallFunc(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    if (IsStdlibFunction(node)) {
        return TranslateCallStdlib(gen, functions, node);
    }
    
    const std::string callFuncName = node->Name();
    node = node->GetRight();

    auto* callingFunc = SearchCallFunc(callFuncName, functions);
    auto* llvmFunction = GetOrCreateFunction(gen, callingFunc);

    // Collect parameters
    std::vector<llvm::Value*> args;
    if (node) {
        node_t* callFuncParams = callingFunc->GetLeft();
        while (node->dType() == DataType::COMMA && callFuncParams->dType() == DataType::COMMA) {
            args.push_back(TransferParamToFunc(gen, functions, node->GetRight(), callFuncParams->GetRight()));
            node = node->GetLeft();
            callFuncParams = callFuncParams->GetLeft();
        }
        args.push_back(TransferParamToFunc(gen, functions, node, callFuncParams));
    }

    std::reverse(args.begin(), args.end());
    return gen.builder.CreateCall(llvmFunction, args);
}

node_t* SearchCallFunc(const std::string& funcName, const std::vector<node_t*>& functions) {
    for (auto* funcNode : functions) {
        if (funcNode->Name() == funcName) {
            return funcNode;
        }
    }
    throw std::runtime_error("Function " + funcName + " not found in std::vector of declared functions!");
}

llvm::Value* TransferParamToFunc(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* param, node_t* paramCallFunc) {
    if (paramCallFunc->dType() == DataType::NEW_VAR) {
        // Directly translate the expression for a new variable
        return TranslateExp(gen, functions, param);
    } else if (param->dType() == DataType::VARIABLE) {
        // Load the variable from memory
        return TranslateVar(param, gen, false);
    } else {
        throw std::runtime_error("Error: Attempt to assign an rvalue to an lvalue");
    }
}


void TranslateInit(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
        
    node = node->GetRight();
    
    if (node->dType() == DataType::VARIABLE) {
        std::string varName = node->Name();
        auto* alloca = gen.builder.CreateAlloca(llvm::Type::getDoubleTy(gen.context), nullptr, varName);
        gen.local_vars[varName] = alloca;
    } 
    else if (node->dType() == DataType::MOV) {
        if (node->GetRight()->dType() == DataType::ARRAY_INIT) {
            TranslateInitArray(gen, functions, node);
        } else {
            TranslateInitVariable(gen, functions, node);
        }
    }
}

void TranslateInitArray(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    assert(node->dType() == DataType::MOV);
    auto* variable = node->GetLeft();
    auto* array    = node->GetRight();
    
    auto  arraySize = GetArraySize(array);
    auto* arrayType = llvm::ArrayType::get(gen.builder.getDoubleTy(), arraySize);
    auto* arrayAlloca = gen.builder.CreateAlloca(arrayType, nullptr, variable->Name());
    gen.local_vars[variable->Name()] = arrayAlloca;

    unsigned i = arraySize - 1;
    node = array->GetLeft();
    std::vector<std::pair<llvm::Value*, llvm::Value*>> values;
    while (node && node->dType() == DataType::COMMA) {
        auto* index = llvm::ConstantInt::get(gen.builder.getInt32Ty(), i--);
        auto* value = TranslateExp(gen, functions, node->GetRight());
        values.emplace_back(index, value);
        node = node->GetLeft();
    }
    auto* index = llvm::ConstantInt::get(gen.builder.getInt32Ty(), i);
    auto* value = TranslateExp(gen, functions, node);
    values.emplace_back(index, value);

    std::reverse(values.begin(), values.end());
    for (auto&& [index, value] : values) {
        auto* elementPtr = gen.builder.CreateGEP(
            arrayType, arrayAlloca,
            {llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.context), 0), index}
        );
        gen.builder.CreateStore(value, elementPtr);
    }
}

size_t GetArraySize(node_t* node) {
    node = node->GetLeft();
    size_t length = 0;
    while (node && node->dType() == DataType::COMMA) {
        node = node->GetLeft();
        length++;
    }
    return ++length;
}

void TranslateInitVariable(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    std::string varName = node->GetLeft()->Name();
    llvm::AllocaInst* alloca = gen.builder.CreateAlloca(llvm::Type::getDoubleTy(gen.context), nullptr, varName);
    gen.local_vars[varName] = alloca;
    TranslateMov(gen, functions, node);
}

void TranslateMov(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    
    auto* rightValue = TranslateExp(gen, functions, node->GetRight());

    std::string leftVarName = node->GetLeft()->Name();

    llvm::Value* leftVar = nullptr;
    auto it = gen.local_vars.find(leftVarName);
    if (it != gen.local_vars.end()) {
        leftVar = it->second;
    } else {
        std::cout << termcolor::red << "Error: " << termcolor::reset
                  << "variable '" << leftVarName << "' is used before it is declared" << std::endl;  
        std::exit(0);      
    }

    gen.builder.CreateStore(rightValue, leftVar);
}

llvm::Value* TranslateRet(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    node = node->GetRight();

    // Copy updated local variables back to their original locations
    unsigned idx = 0;
    for (auto&& arg : gen.current_function->args()) {
        if (arg.getType()->isPointerTy()) {
            auto* updatedValuePtr = gen.local_vars[arg.getName().str()];
            auto* updatedValue = gen.builder.CreateLoad(llvm::Type::getDoubleTy(gen.context), updatedValuePtr);
            gen.builder.CreateStore(updatedValue, &arg);
        }
    }

    llvm::Value* returnValue = TranslateExp(gen, functions, node);
    return gen.builder.CreateRet(returnValue);
}


llvm::Instruction* TranslateCallPrintf(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    node = node->GetRight();

    std::vector<llvm::Value*> args;
    while (node && (node->dType() == DataType::COMMA)) {
        args.push_back(TranslateExp(gen, functions, node->GetRight()));
        node = node->GetLeft();
    }

    if (node) {
        args.push_back(TranslateExp(gen, functions, node));
    }

    for (auto it = args.rbegin(); it != args.rend(); ++it) {
        if ((*it)->getType()->isPointerTy()) {
            PrintString(gen, *it);
        } else {
            PrintOne(gen, *it, "%d ");
        }
    }

    return PrintCharacter(gen, '\n');
}

llvm::Function* GetPrintfFunction(IRGenerator& gen) {
    using namespace llvm;

    // Check if printf is already declared
    auto* printfFunc = gen.module.getFunction("printf");
    if (!printfFunc) {
        // Declare printf function
        auto* printfType = FunctionType::get(Type::getInt32Ty(gen.context), PointerType::get(Type::getInt8Ty(gen.context), 0), true);
        printfFunc = Function::Create(printfType, Function::ExternalLinkage, "printf", gen.module);
    }
    return printfFunc;
}

llvm::Instruction* PrintString(IRGenerator& gen, llvm::Value* strValue) {
    auto* printfFunc = GetPrintfFunction(gen);
    auto* formatStr  = CreateConstantString(gen, "%s ", "format_str");
    return gen.builder.CreateCall(printfFunc, {formatStr, strValue});
}

llvm::Instruction* PrintOne(IRGenerator& gen, llvm::Value* value, const char* format) {
    auto* printfFunc = GetPrintfFunction(gen);
    auto* formatStr  = CreateConstantString(gen, "%lg ", "format_int");
    return gen.builder.CreateCall(printfFunc, {formatStr, value});
}

llvm::Instruction* PrintCharacter(IRGenerator& gen, char character) {
    auto* printfFunc = GetPrintfFunction(gen);
    auto* formatStr  = CreateConstantString(gen, "%c", "format_char");
    auto* charValue  = llvm::ConstantInt::get(llvm::Type::getInt8Ty(gen.context), character);    
    return gen.builder.CreateCall(printfFunc, {formatStr, charValue});
}

llvm::Value* TranslateCallInput(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    node_t* paramSeq = node->GetRight();
    if (paramSeq && NoStringArgument(paramSeq)) {
        return TranslateInputMultipleParameters(gen, paramSeq);
    } else {
        return TranslateInputReturn(gen, functions, node);
    }
}

bool NoStringArgument(node_t* node) {
    if (node->dType() == DataType::COMMA) {
        return NoStringArgument(node->GetLeft()) + (node->GetRight()->dType() != DataType::CONST_STR);
    } else {
        return node->dType() != DataType::CONST_STR;
    }
}

llvm::Value* TranslateInputMultipleParameters(IRGenerator& gen, node_t* node) {
    std::vector<llvm::Value*> variables;
    while (node && node->dType() == DataType::COMMA) {
        variables.push_back(TranslateVar(node->GetRight(), gen, false));
        node = node->GetLeft();
    }

    variables.push_back(TranslateVar(node, gen, false));

    llvm::Value* res = nullptr;
    for (auto* var : variables) {
        res = InputOne(gen, var);
    }

    return res;
}

llvm::Value* InputOne(IRGenerator& gen, llvm::Value* variable) {
    auto* printfFunc = GetInputFunction(gen);
    auto* formatStr  = CreateConstantString(gen, "%lf", "format_double");
    return gen.builder.CreateCall(printfFunc, {formatStr, variable});
}

llvm::Function* GetInputFunction(IRGenerator& gen) {
    using namespace llvm;

    // Check if scanf is already declared
    auto* scanfFunc = gen.module.getFunction("scanf");
    if (!scanfFunc) {
        // Declare scanf function
        auto* scanfType = FunctionType::get(Type::getInt32Ty(gen.context), PointerType::get(Type::getInt8Ty(gen.context), 0), true);
        scanfFunc = Function::Create(scanfType, Function::ExternalLinkage, "scanf", gen.module);
    }
    return scanfFunc;
}

llvm::Value* TranslateInputReturn(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    if (node->GetRight()) {
        TranslateCallPrintf(gen, functions, node);
    }

    auto* tmp = gen.builder.CreateAlloca(llvm::Type::getDoubleTy(gen.context), nullptr);
    InputOne(gen, tmp);
    return gen.builder.CreateLoad(llvm::Type::getDoubleTy(gen.context), tmp);    
}

void TranslateWhile(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    using namespace llvm;

    // Extract the condition and body nodes
    node_t* condition = node->GetLeft();
    node_t* body = node->GetRight();

    // Generate unique names for basic blocks
    auto* function = gen.current_function;
    auto* testBlock = BasicBlock::Create(gen.context, "while_test" + std::to_string(function->size()), function);
    auto* loopBlock = BasicBlock::Create(gen.context, "while_loop" + std::to_string(function->size()), function);
    auto* endBlock  = BasicBlock::Create(gen.context, "while_end"  + std::to_string(function->size()), function);

    // Jump to the test block
    gen.builder.CreateBr(testBlock);

    // Start generating the test block
    gen.builder.SetInsertPoint(testBlock);
    Value* conditionValue = TranslateCondition(gen, functions, condition);

    // Branch based on the condition
    gen.builder.CreateCondBr(conditionValue, loopBlock, endBlock);

    // Start generating the loop body
    gen.builder.SetInsertPoint(loopBlock);
    TranslateOpSequence(gen, functions, body);

    // Jump back to the test block
    gen.builder.CreateBr(testBlock);

    // Continue at the end block
    gen.builder.SetInsertPoint(endBlock);
}

llvm::Value* TranslateCondition(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* condition) {

    // Base case: single condition
    if (condition->dType() != DataType::AND && condition->dType() != DataType::OR) {
        return WriteCmpCondition(gen, functions, condition);
    }

    // Recursive case: AND or OR
    auto* leftCondition = TranslateCondition(gen, functions, condition->GetLeft());
    auto* rightCondition = TranslateCondition(gen, functions, condition->GetRight());

    if (condition->dType() == DataType::AND) {
        return gen.builder.CreateAnd(leftCondition, rightCondition);
    } else { // DataType::OR
        return gen.builder.CreateOr(leftCondition, rightCondition);
    }
}

llvm::Value* WriteCmpCondition(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* condition) {

    auto* lhs = TranslateExp(gen, functions, condition->GetLeft());
    auto* rhs = TranslateExp(gen, functions, condition->GetRight());

    switch (condition->dType()) {
    case DataType::JE:
        return gen.builder.CreateFCmp(llvm::CmpInst::FCMP_UEQ, lhs, rhs);
    case DataType::JNE:
        return gen.builder.CreateFCmp(llvm::CmpInst::FCMP_UNE, lhs, rhs);
    case DataType::JA:
        return gen.builder.CreateFCmp(llvm::CmpInst::FCMP_UGT, lhs, rhs);
    case DataType::JAE:
        return gen.builder.CreateFCmp(llvm::CmpInst::FCMP_UGE, lhs, rhs);
    case DataType::JB:
        return gen.builder.CreateFCmp(llvm::CmpInst::FCMP_ULT, lhs, rhs);
    case DataType::JBE:
        return gen.builder.CreateFCmp(llvm::CmpInst::FCMP_ULE, lhs, rhs);
    default:
        throw std::invalid_argument("Unsupported comparison operator in condition.");
    }
}

void TranslateIf(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    using namespace llvm;

    // Extract condition and branches
    bool hasElseBlock = node->GetRight()->dType() == DataType::ELSE; 
    auto* condition   = node->GetLeft();
    auto* trueBranch  = hasElseBlock ? node->GetRight()->GetLeft()  : node->GetRight();
    auto* falseBranch = hasElseBlock ? node->GetRight()->GetRight() : nullptr;

    // Create blocks
    auto* function = gen.current_function;
    auto* if_true_block = BasicBlock::Create(gen.context, "if_true_" + std::to_string(function->size()), function);
    auto* if_false_block = BasicBlock::Create(gen.context, "if_false_" + std::to_string(function->size()), function);
    auto* if_end_block = BasicBlock::Create(gen.context, "if_end_" + std::to_string(function->size()), function);

    // Translate condition
    auto* condition_value = TranslateCondition(gen, functions, condition);

    // Conditional branch
    gen.builder.CreateCondBr(condition_value, if_true_block, if_false_block);

    // True branch
    gen.builder.SetInsertPoint(if_true_block);
    TranslateOpSequence(gen, functions, trueBranch);
    if (!gen.builder.GetInsertBlock()->getTerminator()) {
        gen.builder.CreateBr(if_end_block);
    }

    // False branch
    gen.builder.SetInsertPoint(if_false_block);
    if (falseBranch && falseBranch->dType() == DataType::IF) {
        TranslateIf(gen, functions, falseBranch);
    } else {
        TranslateOpSequence(gen, functions, falseBranch);
    }
    if (!gen.builder.GetInsertBlock()->getTerminator()) {
        gen.builder.CreateBr(if_end_block);
    }
    
    // End block
    gen.builder.SetInsertPoint(if_end_block);
}

llvm::Value* TranslateStringLiteral(IRGenerator& gen, node_t* node) {
    std::string literal = node->Name();
    std::string literalName = "str_literal_" + std::to_string(gen.string_literals.size());
    return CreateConstantString(gen, literal, literalName);
}

llvm::Value* CreateConstantString(IRGenerator& gen, const std::string& literal, const std::string& literalName) {
    using namespace llvm;
    
    auto it = gen.string_literals.find(literal);
    if (it != gen.string_literals.end()) {
        return it->second;
    }

    auto* strConstant = ConstantDataArray::getString(gen.context, literal, true);
    auto* globalVar = new GlobalVariable( // There is no memory leak, 
        gen.module,                       // gen.module itself manages the memory 
        strConstant->getType(),           // allocated for GlobalVariable
        /* Constant: */ true,
        GlobalValue::PrivateLinkage,
        strConstant,
        literalName
    );

    auto* zero = ConstantInt::get(Type::getInt32Ty(gen.context), 0);
    auto* ptr = gen.builder.CreateGEP(
        strConstant->getType(),
        globalVar,
        {zero, zero}
    );

    gen.string_literals[literal] = ptr;
    return ptr;
}

llvm::Value* TranslateFloatConstant(IRGenerator& gen, node_t* node) {
    return CreateFloatConstant(gen, node->Num());
}

llvm::Value* CreateFloatConstant(IRGenerator& gen, double value) {
    auto it = gen.fp_literals.find(value);
    if (it != gen.fp_literals.end()) {
        return it->second;
    }

    llvm::Constant* float_const = llvm::ConstantFP::get(gen.context, llvm::APFloat(value));
    gen.fp_literals[value] = float_const;
    return float_const;
}

llvm::Value* TranslateExp(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node)
{        
    switch (node->dType())
    {
    case DataType::VARIABLE:
        return TranslateVar(node, gen);
    case DataType::ADD:
        return TranslateAdd(gen, functions, node);
    case DataType::CONST_STR:
        return TranslateStringLiteral(gen, node); 
    case DataType::SUB:
        return TranslateSub(gen, functions, node);
    case DataType::MUL:
        return TranslateMul(gen, functions, node);
    case DataType::DIV:
        return TranslateDiv(gen, functions, node);
    case DataType::DEG:
        return TranslatePow(gen, functions, node);
    case DataType::CONSTANT:
        return TranslateFloatConstant(gen, node);
    case DataType::FUNC:
        return TranslateCallFunc(gen, functions, node);
    case DataType::SCANF:
        return TranslateCallInput(gen, functions, node);
    case DataType::SQRT:
        return TranslateCallSqrt(gen, functions, node); 
    case DataType::LOG:
        return TranslateCallLog(gen, functions, node);
    case DataType::SIN:
        return TranslateCallSin(gen, functions, node);
    case DataType::COS:
        return TranslateCallCos(gen, functions, node);  
    case DataType::ARRAY_ACCESS:
        return TranslateArrayAccess(gen, functions, node);
    default:
        break;
    }

    return nullptr;
}

llvm::Value* TranslateArrayAccess(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* var = node;
    auto* array = node->GetLeft();
    auto* index = array->GetLeft();

    auto it = gen.local_vars.find(var->Name());
    if (it == gen.local_vars.end()) {
        std::cout << termcolor::red << "Error: " << termcolor::reset
                  << "array '" << var->Name() << "' is used before it is declared" << std::endl;
        exit(0);
    }

    auto* arrayAlloca = it->second;
    auto* arrayType = llvm::dyn_cast<llvm::AllocaInst>(arrayAlloca)->getAllocatedType();
    if (!arrayType->isArrayTy()) {
        std::cout << termcolor::red << "Error: " << termcolor::reset
                  << "Variavle '" << var->Name() << "' is not array" << std::endl;
        exit(0);        
    }

    auto* indexValue = TranslateExp(gen, functions, index);
    if (indexValue->getType()->isDoubleTy()) {
        indexValue = gen.builder.CreateFPToUI(indexValue, llvm::Type::getInt32Ty(gen.context));
    }

    auto* elementPtr = gen.builder.CreateGEP(
        arrayType,
        arrayAlloca,
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.context), 0), indexValue}
    );

    auto* elementValue = gen.builder.CreateLoad(
        arrayType->getArrayElementType(),
        elementPtr
    );

    return elementValue;
}

llvm::Value* TranslateVar(node_t* node, IRGenerator& gen, bool load) {
    auto varName = node->Name();
    llvm::Value* var = nullptr;
    if (gen.local_vars.find(varName) != gen.local_vars.end()) {
        auto* var_pointer = gen.local_vars[varName];
        if (load) {
            var = gen.builder.CreateLoad(llvm::Type::getDoubleTy(gen.context), var_pointer);
        } else {
            var = var_pointer;
        }
    } else {
        std::cout << termcolor::red << "Error: " << termcolor::reset
                  << "variable '" << varName << "' is used before it is declared" << std::endl;
        std::exit(0);
    }
    return var;
}

llvm::Value* TranslateAdd(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* leftValue  = TranslateExp(gen, functions, node->GetLeft());
    auto* rightValue = TranslateExp(gen, functions, node->GetRight());
    return gen.builder.CreateFAdd(leftValue, rightValue);
}

llvm::Value* TranslateMul(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* left  = TranslateExp(gen, functions, node->GetLeft());
    auto* right = TranslateExp(gen, functions, node->GetRight());
    return gen.builder.CreateFMul(left, right);
}

llvm::Value* TranslateDiv(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* left  = TranslateExp(gen, functions, node->GetLeft());
    auto* right = TranslateExp(gen, functions, node->GetRight());
    return gen.builder.CreateFDiv(left, right);
}

llvm::Value* TranslatePow(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* base     = TranslateExp(gen, functions, node->GetLeft());
    auto* exponent = TranslateExp(gen, functions, node->GetRight());
    auto* powFunc = llvm::Intrinsic::getDeclaration(&gen.module, llvm::Intrinsic::pow, {base->getType()});
    return gen.builder.CreateCall(powFunc, {base, exponent});
}

llvm::Value* TranslateSub(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    llvm::Value* left;
    if (node->GetLeft()) {
        left = TranslateExp(gen, functions, node->GetLeft());
    } else {
        left = CreateFloatConstant(gen, 0.0);
    }
    auto* right = TranslateExp(gen, functions, node->GetRight());
    return gen.builder.CreateFSub(left, right);
}

llvm::Value* TranslateCallSqrt(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* arg = TranslateExp(gen, functions, node->GetRight());
    auto* sqrtFunc = llvm::Intrinsic::getDeclaration(&gen.module, llvm::Intrinsic::sqrt, {gen.builder.getDoubleTy()});
    return gen.builder.CreateCall(sqrtFunc, arg);
}

llvm::Value* TranslateCallLog(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* arg = TranslateExp(gen, functions, node->GetRight());
    auto* logFunc = llvm::Intrinsic::getDeclaration(&gen.module, llvm::Intrinsic::log, {gen.builder.getDoubleTy()});
    return gen.builder.CreateCall(logFunc, arg);
}

llvm::Value* TranslateCallSin(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* arg = TranslateExp(gen, functions, node->GetRight());
    auto* sinFunc = llvm::Intrinsic::getDeclaration(&gen.module, llvm::Intrinsic::sin, {gen.builder.getDoubleTy()});
    return gen.builder.CreateCall(sinFunc, arg);
}

llvm::Value* TranslateCallCos(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
    auto* arg = TranslateExp(gen, functions, node->GetRight());
    auto* cosFunc = llvm::Intrinsic::getDeclaration(&gen.module, llvm::Intrinsic::cos, {gen.builder.getDoubleTy()});
    return gen.builder.CreateCall(cosFunc, arg);
}