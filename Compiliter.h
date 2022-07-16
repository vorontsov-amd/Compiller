#pragma once
#include "Differenciator.h"
#include "List.h"
#include "main.h"
#include "ByteArray.h"
#include "Variable.h"




void TranslateToAsm(List<DifferTree>& proga);
char* ProgrammName(List<DifferTree>& proga);
char* MainFuncName(List<DifferTree>& proga);
void WritePreamble(FILE* fasm, List<DifferTree>& proga);
void TranslateProcessing(FILE* fasm, List<DifferTree>& proga, char* programm_name, ByteArray& machine_code);
void TreeTranslate(FILE* fasm, DifferTree& function, char* programm_name, ByteArray& machine_code);
void VerifyDefFunc(DifferTree& function);
inline char* NodeName(node_t* function);
void NumVar(int& num_param, node_t* func);
List<variable>* PrintProlog(FILE* fasm, DifferTree& function, ByteArray& machine_code);
void VerifyFunc(node_t* node);
List<variable>* PrintDefineParam(FILE* fasm, node_t* node);
int NumParam(node_t* node);
void TranslateOpSequence(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset);
void TranslateOp(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset);
void TranslateCallFunc(FILE* fasm, List<variable>* lst, node_t* node, char* funcname);
void TranslateInit(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset);
void TranslateMov(FILE* fasm, List<variable>* param, node_t* node, char* funcname);
void TranslateExp(FILE* fasm, List<variable>* param, node_t* node, char* funcname, bool st_exp = true);
void PrintEpilog(FILE* fasm, List<variable>* param, node_t* node, char* funcname);
void PrintConstant(FILE* fasm, List<DifferTree> proga);
void SearchConst(FILE* fasm, node_t* node);
void TranslateIf(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset);
const char* Jnx(node_t* node);
const char* Jxx(node_t* node);
void TranslateWhile(FILE* fasm, List<variable>* param,  node_t* node, char* funcname, int offset);
void TranslateCallPrintf(FILE* fasm, List<variable>* lst, node_t* node, char* funcname);
void TranslateCallScanf(FILE* fasm, List<variable>* lst, node_t* node, char* funcname);
void TransateCallSqtr(FILE* fasm, List<variable>* lst, node_t* node, char* funcname);
void TranslateRet(FILE* fasm, List<variable>* param, node_t* node, char* funcname);
void SaveFPU(FILE* fasm);
void LoadFPU(FILE* fasm);
void TTHex_sub_rsp_number(unsigned long long offset, ByteArray& machine_code);
uint64_t OffsetVariable(List<variable>* lst, node_t* var_ptr);