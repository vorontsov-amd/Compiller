#pragma once
#include "Differenciator.h"
#include "List.h"
#include "main.h"
#include "ByteArray.h"
#include "Variable.h"
#include "exceptions.h"





void TranslateToAsm(List<DifferTree>& proga);
const char* ProgrammName(List<DifferTree>& proga);
void StrToHex(const char* str, ByteArray& machine_code);
const char* MainFuncName(List<DifferTree>& proga);
void WritePreamble(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code);
void PreambleRodata(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code);
void PreambleData(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code);
void TranslateProcessing(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code);
List<node_t>* CreateLstFuncNode(List<DifferTree> proga);
void TreeTranslate(FILE* fasm, DifferTree& function, List<node_t>* functions, Stubs& stubs, ByteArray& machine_code);
void VerifyDefFunc(node_t* function);
void NumLocalVar(int& num_param, node_t* func);
List<variable>* WriteFuncProlog(FILE* fasm, node_t* function, Stubs& stubs, ByteArray& machine_code);
void VerifyFunc(node_t* node);
List<variable>* FillListVariables(node_t* node);
int NumParam(node_t* node);
uint32_t SizeStackFrame(node_t* func, List<variable>* variables);
void CopyParametrsToStack(FILE* fasm, List<variable>* variables, ByteArray& machine_code);
void TranslateOpSequence(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code);
void TranslateOp(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code);
void TranslateCallFunc(FILE* fasm, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code);
node_t SearchCallFunc(const char* func_name, List<node_t>* functions);
void TransferParamToFunc(FILE* fasm, node_t* param, node_t param_call_func, List<node_t>* functions, List<variable>* lst, const char* funcname, Stubs& stubs, ByteArray& machine_code);
void TranslateInit(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code);
void TranslateMov(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code);
void TranslateExp(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code);
void WriteFuncEpilog(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code);
void WriteConstant(FILE* fasm, List<DifferTree> proga, Stubs& stubs, ByteArray& machine_code);
void SearchConst(FILE* fasm, node_t* node, Stubs& stubs, ByteArray& machine_code);
void AppendConst(FILE* fasm, node_t* node, Stubs& stubs, ByteArray& machine_code);
void TranslateIf(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code);
const char* Jnx(node_t* node);
const char* Jxx(node_t* node);
void TranslateWhile(FILE* fasm, List<node_t>* functions, List<variable>* param,  node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code);
void TranslateCallPrintf(FILE* fasm, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code);
void PrintOne(FILE* fasm, Stubs& stubs, ByteArray& machine_code);
void PrintCharacter(FILE* fasm, uint32_t character, ByteArray& machine_code);
void TranslateCallScanf(FILE* fasm, List<variable>* lst, node_t* node, Stubs& stubs, ByteArray& machine_code);
void TranslateScanfReturn(FILE* fasm, node_t* node, Stubs& stubs, ByteArray& machine_code);
void TranslateScanfMoreParametrs(FILE* fasm, List<variable>* lst, node_t* node, Stubs& stubs, ByteArray& machine_code);
void TranslateBaseScanf(FILE* fasm, Stubs& stubs, ByteArray& machine_code);
void ScanOne(FILE* fasm, uint64_t offset, Stubs& stubs, ByteArray& machine_code);
void TransateCallSqtr(FILE* fasm, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code);
void TranslateRet(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_codee);
uint64_t OffsetVariable(List<variable>* lst, node_t* var_ptr);
void WriteELFHeader(Stubs& stubs, ByteArray& machine_code);
void Test(ByteArray& machine_code);
void WriteProgrammProlog(FILE* fasm, List<DifferTree>& tree, Stubs& stubs, ByteArray& machine_code);
void WriteStdFunctions(Stubs& stubs, ByteArray& machine_code);
void WritePow(Stubs& stubs, ByteArray& machine_code);
void WriteLog10(Stubs& stubs, ByteArray& machine_code);
void WriteIsNan(Stubs& stubs, ByteArray& machine_code);
void WriteIsInf(Stubs& stubs, ByteArray& machine_code);
void WriteStrlen(Stubs& stubs, ByteArray& machine_code);
void WriteFloor(Stubs& stubs, ByteArray& machine_code);
void WriteDtoa(Stubs& stubs, ByteArray& machine_code);




