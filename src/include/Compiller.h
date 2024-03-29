#pragma once
#include "Differentiator.h"
#include "List.h"
#include "main.h"
#include "ByteArray.h"
#include "Variable.h"



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
List<node_t>* CreateLstFuncNode(List<DifferTree>& proga);
void TreeTranslate(FILE* fasm, DifferTree& function, List<node_t>* functions,  ByteArray& machine_code);
void VerifyDefFunc(node_t* function);
void NumLocalVar(int& num_param, node_t* func);
List<variable>* WriteFuncProlog(FILE* fasm, node_t* function,  ByteArray& machine_code);
void VerifyFunc(node_t* node);
List<variable>* FillListVariables(node_t* node);
int NumParam(node_t* node);
uint32_t SizeStackFrame(node_t* func, List<variable>* variables);
void CopyParametrsToStack(FILE* fasm, List<variable>* variables, ByteArray& machine_code);
void TranslateOpSequence(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset,  ByteArray& machine_code);
void TranslateOp(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset,  ByteArray& machine_code);
void TranslateCallFunc(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
node_t SearchCallFunc(const char* func_name, List<node_t>* functions);
void TransferParamToFunc(FILE* fasm, int& num_const_str, node_t* param, node_t param_call_func, List<node_t>* functions, List<variable>* lst, const char* funcname,  ByteArray& machine_code);
void TranslateInit(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset,  ByteArray& machine_code);
void TranslateMov(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateExp(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void WriteFuncEpilog(FILE* fasm, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void WriteConstant(FILE* fasm, DataType::dataType mode, List<DifferTree>& proga, ByteArray& machine_code);
void SearchConst(FILE* fasm, DataType::dataType mode, node_t* node, ByteArray& machine_code);
void AppendConst(FILE* fasm, node_t* node, ByteArray& machine_code);
void AppendStr(FILE* fasm, node_t* node, ByteArray& machine_code);
void TranslateIf(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset,  ByteArray& machine_code);
node_t* TranslateIfCondSeq(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, node_t* parrent, const ThreeIfStr& strings, bool have_else, const char* funcname,  ByteArray& machine_code);
void WriteCmpCondition(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
const char* Jnx(node_t* node);
const char* Jxx(node_t* node);
void TranslateWhile(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param,  node_t* node, const char* funcname, int offset,  ByteArray& machine_code);
node_t* TranslateWhileCondSeq(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, node_t* parrent, const TwoWhileStr& strings, const char* funcname,  ByteArray& machine_code);
void TranslateCallPrintf(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
void PrintOne(FILE* fasm,  ByteArray& machine_code);
void PrintCharacter(FILE* fasm, uint32_t character, ByteArray& machine_code);
void PrintString(FILE* fasm, int num_const_str,  ByteArray& machine_code);
bool NoStringArgument(node_t* node);
void TranslateCallScanf(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateScanfReturn(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateScanfMoreParametrs(FILE* fasm, List<variable>* lst, node_t* node,  ByteArray& machine_code);
void TranslateBaseScanf(FILE* fasm,  ByteArray& machine_code);
void ScanOne(FILE* fasm, uint64_t offset,  ByteArray& machine_code);
void TranslateCallSqtr(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateRet(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_codee);
uint64_t OffsetVariable(List<variable>* lst, node_t* var_ptr);
void WriteProgrammProlog(FILE* fasm, List<DifferTree>& tree, ByteArray& machine_code);
void TranslatePow(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateMul(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateAdd(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateDiv(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateSub(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
inline void TranslateVar(FILE* fasm, List<variable>* param, node_t* node, ByteArray& machine_code);
void TranslateCallLog(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateCallCos(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateCallSin(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname,  ByteArray& machine_code);
void TranslateConst(FILE* fasm, ByteArray& machine_code);
inline void GetOneArgument(FILE* fasm, ByteArray& code);
inline void GiveArgument(FILE* fasm, ByteArray& code);
inline void GetTwoArgument(FILE* fasm, ByteArray& code);