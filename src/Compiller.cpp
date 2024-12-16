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

void TranslateToAsm(List<DifferTree>& proga, const char* out_name)
{
    const char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");

    ByteArray machine_code;

    machine_code.AppendElfHeader();
    WritePreamble(fasm, proga, machine_code);
    TranslateProcessing(fasm, proga,  machine_code);

    // rewind(fasm);
    // machine_code.Reset();

    // machine_code.AppendElfHeader();
    // WritePreamble(fasm, proga, machine_code);
    // TranslateProcessing(fasm, proga,  machine_code);

    // FILE* out = fopen(out_name, "wb");    
    // assert(out);
    // fwrite(machine_code.ByteCode(), 1, machine_code.Size(), out);
    // fclose(fasm);
    // fclose(out);
    // delete[] programm_name;
}

const char* ProgrammName(List<DifferTree>& proga)
{
    const char* name = FrontFuncName(proga);
    char* fullname = new char[strlen(name) + 5];
    strcpy(fullname, name);
    strcat(fullname, ".asm");
    return fullname;
}

const char* FrontFuncName(List<DifferTree>& proga)
{
    DifferTree main_function = proga.ShowFront();
    node_t function = main_function.Root()->GetRight();
    return function.Name();
}

void WritePreamble(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code)
{
    fputs("global _start\n", fasm);
    fputs("extern dtoa, atod, pow\n\n", fasm);
    PreambleData(fasm, proga, machine_code);
    PreambleRodata(fasm, proga, machine_code);
    fputs("section .text\n", fasm);
}

void PreambleRodata(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code)
{
    fputs("section .rodata\n", fasm);

    uint64_t rodata_begin = machine_code.Size();
    if (machine_code.stubsNotLoaded())
    {
        machine_code.rodataStubs().p_offset = rodata_begin;
        machine_code.rodataStubs().p_vaddp  = machine_code.e_point() + rodata_begin;
    }

    WriteConstant(fasm, DataType::CONSTANT, proga, machine_code);
    WriteConstant(fasm, DataType::CONST_STR, proga, machine_code);


    uint16_t rodata_end = machine_code.Size();
    if (machine_code.stubsNotLoaded())
    {
        machine_code.rodataStubs().p_size = rodata_end - rodata_begin;
    }
}

void PreambleData(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code)
{
    fputs("section .data\n", fasm);

    if (machine_code.stubsNotLoaded())
    {
        machine_code.dataStubs().p_offset = machine_code.Size();
        machine_code.dataStubs().p_vaddp = machine_code.e_point() + machine_code.Size();
        machine_code.dataStubs().p_size = sizeof(double);
    }

    fputs("buffer: dq 0.0\n", fasm);
    machine_code.Append(0.0);

    fputs("str: times 32 db 0\n", fasm);
    machine_code.Append(0U);
    machine_code.Append(0U);
    machine_code.Append(0U);
    machine_code.Append(0U);
}

void WriteConstant(FILE* fasm, DataType::dataType mode, List<DifferTree>& proga, ByteArray& machine_code)
{       
    for (auto it = proga.begin(); it != proga.end(); ++it)
    {
        SearchConst(fasm, mode, it->Root(), machine_code);
    }
}

void SearchConst(FILE* fasm, DataType::dataType mode, node_t* node, ByteArray& machine_code)
{
    if (node->GetLeft()) SearchConst(fasm, mode, node->GetLeft(), machine_code);
    if (node->dType() == mode)
    {
        auto AppendData = AppendConst;
        switch (mode)
        {
        case DataType::CONST_STR:
            AppendData = AppendStr;
            break;
        case DataType::CONSTANT:
            AppendData = AppendConst;
            break;
        default:
            fprintf(stderr, "Incorrect Searching mode");
            exit(EXIT_FAILURE);
            break;
        }
        AppendData(fasm, node, machine_code);
    }
    if (node->GetRight()) SearchConst(fasm, mode, node->GetRight(), machine_code);
}

void AppendConst(FILE* fasm, node_t* node, ByteArray& machine_code)
{    
    static int num_const = 0;
    if (machine_code.resetConstDeclCounter())
    {
        num_const = 0;
    }

    double number = node->Num();
    std::string lbl_number = "const_" + std::to_string(num_const);

    fprintf(fasm, "%s: dq %lf\n", lbl_number.c_str(), number);

    machine_code.AddLabel(lbl_number);
    machine_code.Append(number);

    num_const++;
}

void AppendStr(FILE* fasm, node_t* node, ByteArray& machine_code)
{
    static int num_const = 0;
    if (machine_code.resetStrDeclCounter())
    {
        num_const = 0;
    }

    const char* str = node->Name();
    std::string str_mark = "str_" + std::to_string(num_const);

    fprintf(fasm, "%s: db '", str_mark.c_str());
    fputs(str, fasm);
    fputs("'\n", fasm);

    uint32_t str_len = strlen(str);

    machine_code.AddLabel(str_mark);
    machine_code.Append(str_len);
    machine_code.Append(str, str_len);

    num_const++;
}

void TranslateProcessing(FILE* fasm, List<DifferTree> proga,  ByteArray& machine_code)
{
    const std::vector<node_t*>& functions = CreateLstFuncNode(proga);
    

    IRGenerator generator;

    int size = proga.Size();
    for (int i = 0; i < size; i++)
    {
        DifferTree function = proga.ShowFront();
        TreeTranslate(function, functions, generator);
        proga.PopFront();
    }

    generator.dump();
}

std::vector<node_t*> CreateLstFuncNode(List<DifferTree>& proga)
{
    std::vector<node_t*> lst;

    for (auto it = proga.begin(); it != proga.end(); ++it)
    {        
        lst.push_back(it->Root()->GetRight());
    }

    return lst;
}

void WriteProgrammProlog(FILE* fasm, List<DifferTree>& tree, ByteArray& machine_code)
{   
    const char* programm_name = FrontFuncName(tree);
    
    fprintf(fasm, "_start:\n");
    fprintf(fasm, "\t\tfinit\n");
    fprintf(fasm, "\t\tcall\t%s\n", programm_name);

    fprintf(fasm, "\t\tmov\t\trax, 60\n");
    fprintf(fasm, "\t\txor\t\trdi, rdi\n");
    fprintf(fasm, "\t\tsyscall\n");

    machine_code.AppendCmd(CMD::FINIT, 3);
    machine_code.AppendCallFunc(programm_name);
    machine_code.AppendCmd(CMD::MOV_RAX_60, 7);
    machine_code.AppendCmd(CMD::XOR_RDI_RDI, 3);
    machine_code.AppendCmd(CMD::SYSCALL, 2);
}

void VerifyDefFunc(node_t* function)
{
    if (function->dType() != DataType::DEFINE)
    {
        std::cout << *function << "\n";
        fprintf(stderr, "Error reading the function tree\n");
        exit(EXIT_FAILURE);
    }
}

void TreeTranslate(DifferTree& function, const std::vector<node_t*>& functions, IRGenerator& gen)
{
    node_t* func = function.Root();
    VerifyDefFunc(func);

    func = func->GetRight();
    auto* llvm_function = WriteFuncProlog(func, gen);
    gen.current_function = llvm_function;

    TranslateOpSequence(gen, functions, func->GetRight());

    WriteFuncEpilog(llvm_function, gen);
}

llvm::Function* WriteFuncProlog(node_t* func, IRGenerator& gen) {
    using namespace llvm;

    auto funcName = func->Name();
    auto variables = FillListVariables(func);
    std::vector<Type*> argTypes;
    for (auto variable : variables) {
        if (variable.IsLink()) {
            argTypes.push_back(Type::getDoubleTy(gen.context)->getPointerTo());
        } else {
            argTypes.push_back(Type::getDoubleTy(gen.context));
        }
    }

    auto* funcType = FunctionType::get(Type::getDoubleTy(gen.context), argTypes, false);


    auto* llvmFunc = gen.module.getFunction(funcName);
    if (!llvmFunc) {
        llvmFunc = Function::Create(funcType, Function::ExternalLinkage, funcName, &gen.module);
    }

    unsigned idx = 0;
    for (auto&& arg : llvmFunc->args()) {
        arg.setName(variables[idx].Name());
        ++idx;
    }

    auto* entryBlock = BasicBlock::Create(gen.context, "entry", llvmFunc);
    
    gen.builder.SetInsertPoint(entryBlock);

    // Copy arguments to stack
    idx = 0;
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
    
    // auto* returnBlock = BasicBlock::Create(gen.context, "return", llvmFunc);
    // gen.builder.SetInsertPoint(returnBlock);

    // Copy updated local variables back to their original locations
    unsigned idx = 0;
    for (auto&& arg : llvmFunc->args()) {
        if (arg.getType()->isPointerTy()) {
            auto* updatedValuePtr = gen.local_vars[arg.getName().str()];
            auto* updatedValue = gen.builder.CreateLoad(Type::getDoubleTy(gen.context), updatedValuePtr);
            gen.builder.CreateStore(updatedValue, &arg);
        }
    }

    auto zero = CreateFloatConstant(gen, 0.0);
    gen.builder.CreateRet(zero);
}

std::vector<variable> FillListVariables(node_t* node)
{
    VerifyFunc(node);
    std::vector<variable> variables;

    if (node->GetLeft())
    {
        int offset = NumParam(node) * sizeof(double);
        node = node->GetLeft();
        while (node->dType() == DataType::COMMA)
        {
            node_t* var_ptr = node->GetRight();
            if (var_ptr->dType() == DataType::NEW_VAR)
            {
                var_ptr = var_ptr->GetRight();
                variable var(var_ptr, false, offset);
                variables.push_back(var);
            }
            else
            {
                variable var(var_ptr, true, offset);
                variables.push_back(var);
            }
            offset -= sizeof(double);
            node = node->GetLeft();
        }
        if (node->dType() == DataType::NEW_VAR)
        {
            node = node->GetRight();
            variable var(node, false, offset);
            variables.push_back(var);
        }
        else
        {
            variable var(node, true, offset);
            variables.push_back(var);
        }
    }

    return variables;
}

int NumParam(node_t* node)
{
    int num_param = 0;
    while (node->GetLeft())
    {
        num_param++;
        node = node->GetLeft();
    }
    return num_param;
}

void NumLocalVar(int& num_param, node_t* node)
{    
    if (node->dType() == DataType::INITIALIZATE)
    {
        num_param++;
        return;
    }
    if (node->GetLeft()) NumLocalVar(num_param, node->GetLeft());
    if (node->GetRight()) NumLocalVar(num_param, node->GetRight());

}

void VerifyFunc(node_t* node)
{
    if (node->dType() != DataType::FUNC)
    {
        std::cout << node << "\n";
        fprintf(stderr, "Error reading function parametrs");
        exit(EXIT_FAILURE);
    }
}

uint32_t SizeStackFrame(node_t* func, List<variable>* variables)
{
    int num_variables = 0 ;
    NumLocalVar(num_variables, func);
    num_variables += variables->Size();
    return num_variables * sizeof(double);
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

        auto variables = FillListVariables(func);

        std::vector<Type*> argTypes;
        for (auto variable : variables) {
            if (variable.IsLink()) {
                argTypes.push_back(Type::getDoubleTy(gen.context)->getPointerTo());
            } else {
                argTypes.push_back(Type::getDoubleTy(gen.context));
            }
        }

        auto* funcType = FunctionType::get(Type::getDoubleTy(gen.context), argTypes, false);
        targetFunc = Function::Create(funcType, Function::ExternalLinkage, funcName, &gen.module);
    }

    return targetFunc;
}

llvm::Value* TranslateCallFunc(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node) {
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
    throw std::runtime_error("Function " + funcName + " not found in list of declared functions!");
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
        std::string varName = node->GetLeft()->Name();
        llvm::AllocaInst* alloca = gen.builder.CreateAlloca(llvm::Type::getDoubleTy(gen.context), nullptr, varName);
        gen.local_vars[varName] = alloca;
        TranslateMov(gen, functions, node);
    }
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
    auto* testBlock = BasicBlock::Create(gen.context, "while_test" + function->size(), function);
    auto* loopBlock = BasicBlock::Create(gen.context, "while_loop" + function->size(), function);
    auto* endBlock  = BasicBlock::Create(gen.context, "while_end"  + function->size(), function);

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
    auto* condition = node->GetLeft();
    auto* trueBranch = node->GetRight()->GetLeft();
    auto* falseBranch = node->GetRight()->GetRight();

    // Create blocks
    auto* function = gen.current_function;
    auto* if_true_block = BasicBlock::Create(gen.context, "if_true_" + function->size(), function);
    auto* if_false_block = BasicBlock::Create(gen.context, "if_false_" + function->size(), function);
    auto* if_end_block = BasicBlock::Create(gen.context, "if_end_" + function->size(), function);

    // Translate condition
    auto* condition_value = TranslateCondition(gen, functions, condition);

    // Conditional branch
    gen.builder.CreateCondBr(condition_value, if_true_block, if_false_block);

    // True branch
    gen.builder.SetInsertPoint(if_true_block);
    TranslateOpSequence(gen, functions, trueBranch);
    gen.builder.CreateBr(if_end_block);

    // Add false block to the function
    gen.builder.SetInsertPoint(if_false_block);
    if (falseBranch) {
        TranslateOpSequence(gen, functions, falseBranch);
    }
    gen.builder.CreateBr(if_end_block);

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
    default:
        break;
    }

    return nullptr;
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