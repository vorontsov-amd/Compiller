#pragma once
#include "Differenciator.h"
#include "lexical_analysis.h"
#include "Compiliter.h"
#include "Reading.h"
#include "sys/stat.h"
#include "stdlib.h"


List<node_t> ListScan(const char* filename);
size_t Filesize  (FILE *stream);
void ClearBuf(char* buffer, size_t filesize, bool* flag);
void ProgrammDump(List<DifferTree> tree);
char* NameFunc(DifferTree tree);