#include "Differentiator.h"
#include "lexical_analysis.h"
#include "Compiller.h"
#include "Reading.h"
#include "sys/stat.h"
#include "stdlib.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

List<node_t> ListScan(const char* filename);
size_t Filesize  (FILE *stream);
void ClearBuf(char* buffer, size_t filesize, bool* flag);
po::variables_map GetArguments(int argc, const char* argv[]);