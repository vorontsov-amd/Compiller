#pragma once
#include "Differenciator.h"
#include "List.h"


void TranslateToAsm(List<DifferTree>& proga);
char* ProgrammName(List<DifferTree>& proga);
char* MainFuncName(List<DifferTree>& proga);
void WritePreamble(FILE* fasm, List<DifferTree>& proga);
void TranslateProcessing(FILE* fasm, List<DifferTree>& proga);
void TreeTranslate(FILE* fasm, DifferTree& function);
void VerifyDefFunc(DifferTree& function);
inline char* NodeName(node_t* function);
void NumVar(int& num_param, node_t* func);
void PrintProlog(FILE* fasm, DifferTree& function);
void VerifyFunc(node_t* node);
void PrintDefineParam(FILE* fasm, node_t* node);
int NumParam(node_t* node);