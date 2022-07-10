#pragma once
#include "Differenciator.h"
#include "List.h"
#include "main.h"


void TranslateToAsm(List<DifferTree>& proga);
char* ProgrammName(List<DifferTree>& proga);
char* MainFuncName(List<DifferTree>& proga);
void WritePreamble(FILE* fasm, List<DifferTree>& proga);
void TranslateProcessing(FILE* fasm, List<DifferTree>& proga, char* programm_name);
void TreeTranslate(FILE* fasm, DifferTree& function, char* programm_name);
void VerifyDefFunc(DifferTree& function);
inline char* NodeName(node_t* function);
void NumVar(int& num_param, node_t* func);
void PrintProlog(FILE* fasm, DifferTree& function);
void VerifyFunc(node_t* node);
void PrintDefineParam(FILE* fasm, node_t* node);
int NumParam(node_t* node);
void TranslateOpSequence(FILE* fasm, node_t* node, char* funcname);
void TranslateOp(FILE* fasm, node_t* node, char* funcname);
void TranslateCallFunc(FILE* fasm, node_t* node, char* funcname);
void TranslateInit(FILE* fasm, node_t* node, char* funcname);
void TranslateMov(FILE* fasm, node_t* node, char* funcname);
void TranslateExp(FILE* fasm, node_t* node, char* funcname);
void PrintEpilog(FILE* fasm);
void PrintConstant(FILE* fasm, List<DifferTree> proga);
void SearchConst(FILE* fasm, node_t* node);
void TranslateIf(FILE* fasm, node_t* node, char* funcname);
const char* Jnx(node_t* node);
