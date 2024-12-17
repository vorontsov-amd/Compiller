#pragma once
#include "Differentiator.h"
#include "main.h"
#include "IRGenerator.hpp"
#include <vector>

void TranslateToAsm(std::vector<DifferTree*>& proga, const char* out_name);
void TranslateProcessing(std::vector<DifferTree*>& proga);
std::vector<node_t*> CreateLstFuncNode(std::vector<DifferTree*>& proga);
void TreeTranslate(DifferTree* function, const std::vector<node_t*>& functions, IRGenerator& gen);
void DeclareStdlib(IRGenerator& gen);
bool IsStdlibFunction(node_t* node);
llvm::Value* TranslateCallStdlib(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateArrayAccess(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateInitVariable(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateInitArray(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
size_t GetArraySize(node_t* node);
auto GetArgumentInfo(IRGenerator& gen, node_t* node);


llvm::Function* WriteFuncProlog(node_t* func, IRGenerator& gen);
void WriteFuncEpilog(llvm::Function* llvmFunc, IRGenerator& gen);
llvm::Value* TranslateVar(node_t* node, IRGenerator& gen, bool load = true);
llvm::Value* CreateConstantString(IRGenerator& gen, const std::string& literal, const std::string& literalName);


llvm::Value* TransferParamToFunc(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* param, node_t* paramCallFunc);
void TranslateOpSequence(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateOp(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Function* GetOrCreateFunction(IRGenerator& gen, node_t* func);
llvm::Value* TranslateCallFunc(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
node_t* SearchCallFunc(const std::string& funcName, const std::vector<node_t*>& functions);
llvm::Value* TransferParamToFunc(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* param, node_t* paramCallFunc);
void TranslateInit(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateMov(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateExp(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateIf(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* WriteCmpCondition(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* condition);
void TranslateWhile(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateCondition(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* condition);
llvm::Instruction* TranslateCallPrintf(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Function* GetPrintfFunction(IRGenerator& gen);
llvm::Instruction* PrintOne(IRGenerator& gen, llvm::Value* value, const char* format);
llvm::Instruction* PrintCharacter(IRGenerator& gen, char character);
llvm::Instruction* PrintString(IRGenerator& gen, llvm::Value* strValue);
bool NoStringArgument(node_t* node);
llvm::Value* TranslateCallInput(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateInputReturn(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateInputMultipleParameters(IRGenerator& gen, node_t* node);
llvm::Function* GetInputFunction(IRGenerator& gen);
llvm::Value* InputOne(IRGenerator& gen, llvm::Value* variable);
llvm::Value* TranslateRet(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslatePow(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateMul(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateAdd(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateDiv(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateSub(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateCallLog(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateCallCos(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateCallSin(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateCallSqrt(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateFloatConstant(IRGenerator& gen, node_t* node);
llvm::Value* CreateFloatConstant(IRGenerator& gen, double value);