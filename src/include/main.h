#include "Differentiator.h"
#include "lexical_analysis.h"
#include "Compiller.h"
#include "Reading.h"
#include "sys/stat.h"
#include "stdlib.h"

std::deque<node_t> Scan(const char* filename);
size_t Filesize  (FILE *stream);
void ClearBuf(char* buffer, size_t filesize, bool* flag);
void ProgrammDump(std::vector<DifferTree*>& tree);
void VerifyMainArgument(int argc, const char* argv[]);