#pragma once
#include "Differentiator.h"
#include "List.h"
#include "main.h"
#include "ByteArray.h"
#include "Variable.h"
#include "IRGenerator.hpp"
#include <vector>


struct ThreeIfStr
{
    const char* if_start;
    const char* if_else;
    const char* if_end;
};

struct TwoWhileStr
{
    const char* while_loop;
    const char* while_end;
};



void TranslateToAsm(List<DifferTree>& proga, const char* out_name);
const char* ProgrammName(List<DifferTree>& proga);
const char* FrontFuncName(List<DifferTree>& proga);
void WritePreamble(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code);
void PreambleRodata(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code);
void PreambleData(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code);
void TranslateProcessing(FILE* fasm, List<DifferTree> proga,  ByteArray& machine_code);
std::vector<node_t*> CreateLstFuncNode(List<DifferTree>& proga);
void TreeTranslate(DifferTree& function, const std::vector<node_t*>& functions, IRGenerator& gen);
void VerifyDefFunc(node_t* function);
void NumLocalVar(int& num_param, node_t* func);

llvm::Function* WriteFuncProlog(node_t* func, IRGenerator& gen);
void WriteFuncEpilog(llvm::Function* llvmFunc, IRGenerator& gen);
std::vector<variable> FillListVariables(node_t* node);
llvm::Value* TranslateVar(node_t* node, IRGenerator& gen, bool load = true);
llvm::Value* CreateConstantString(IRGenerator& gen, const std::string& literal, const std::string& literalName);

void VerifyFunc(node_t* node);

int NumParam(node_t* node);
uint32_t SizeStackFrame(node_t* func, List<variable>* variables);
void CopyParametrsToStack(FILE* fasm, List<variable>* variables, ByteArray& machine_code);
void TranslateOpSequence(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateOp(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateCallFunc(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
node_t SearchCallFunc(const char* func_name, List<node_t>* functions);
void TransferParamToFunc(FILE* fasm, int& num_const_str, node_t* param, node_t param_call_func, List<node_t>* functions, List<variable>* lst, const char* funcname,  ByteArray& machine_code);
void TranslateInit(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void TranslateMov(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateExp(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
void WriteConstant(FILE* fasm, DataType::dataType mode, List<DifferTree>& proga, ByteArray& machine_code);
void SearchConst(FILE* fasm, DataType::dataType mode, node_t* node, ByteArray& machine_code);
void AppendConst(FILE* fasm, node_t* node, ByteArray& machine_code);
void AppendStr(FILE* fasm, node_t* node, ByteArray& machine_code);
void TranslateIf(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* WriteCmpCondition(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* condition);
void TranslateWhile(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* node);
llvm::Value* TranslateCondition(IRGenerator& gen, const std::vector<node_t*>& functions, node_t* condition);
node_t* TranslateWhileCondSeq(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, node_t* parrent, const TwoWhileStr& strings, const char* funcname,  ByteArray& machine_code);
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
uint64_t OffsetVariable(List<variable>* lst, node_t* var_ptr);
void WriteProgrammProlog(FILE* fasm, List<DifferTree>& tree, ByteArray& machine_code);
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