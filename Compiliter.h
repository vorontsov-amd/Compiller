#pragma once
#include "Differenciator.h"
#include "List.h"


void TranslateToAsm(List<DifferTree>& proga);
char* ProgrammName(List<DifferTree>& proga);
char* MainFuncName(List<DifferTree>& proga);
void WritePreamble(FILE* fasm, List<DifferTree>& proga);
void TranslateProcessing(List<DifferTree>& proga);
void TreeTranslate(DifferTree& function);
void VerifyFunc(DifferTree& function);