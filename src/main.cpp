#include "main.h"
#include "termcolor/termcolor.hpp"
#include <iostream>


int main(int argc, char const *argv[])
{    
    auto arguments = GetArguments(argc, argv);
    auto input_file = arguments["input-file"].as<std::string>();
    List<node_t> lst = ListScan(input_file.c_str());
	List<DifferTree> tree = GetGrammar(lst);
    TranslateProcessing(tree, arguments);
}

po::variables_map GetArguments(int argc, const char* argv[])
{
    po::options_description desc("Compiler options");
    desc.add_options()
        ("help,h", "Show help message")
        ("S,S", "Compile only. Do not assemble or link.")
        ("output,o", po::value<std::string>(), "Output file")
        ("input-file", po::value<std::string>(), "Input file for compilation");
        ("library-path,L", po::value<std::string>(), "Add standsrt library search path");

    po::positional_options_description p;
    p.add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }

    if (!vm.count("input-file")) {
        std::cerr << "Input file is required!" << std::endl;
        exit(0);
    }

    return vm;
}

size_t Filesize(FILE *stream)  
{
    struct stat buff = {};

    fstat(fileno(stream), &buff);
    
    return buff.st_size;
}

List<node_t> ListScan(const char* filename)
{
    FILE* stream = fopen(filename, "r");
	
	size_t filesize = Filesize(stream);

    char* buffer = new (std::nothrow) char[filesize + 1];
    if (buffer == nullptr)
    {
        fprintf(stderr, "buffer pointer nullptr. Line: %d\n", __LINE__);
        fclose(stream);
        exit(EXIT_FAILURE);
    }

    size_t count = fread(buffer, sizeof(char), filesize, stream);
    if (count != filesize)
    {
        fprintf(stderr, "count fread() return != filesize. Line: %d", __LINE__);
        fclose(stream);
        exit(EXIT_FAILURE);
    }

    buffer[filesize] = '\0';

    fclose(stream);

	for (int i = 0; i <= filesize; i++)
	{
		if (buffer[i] == '\r' || buffer[i] == '\n' || buffer[i] == '\t')
		{
			buffer[i] = ' ';
        }
    }
    bool flag = false;
    do
    {
        flag = false;
        ClearBuf(buffer, filesize, &flag);
    } while (flag);
    
    return AnalysProcessing(buffer, filesize + 1);
}


void ClearBuf(char* buffer, size_t filesize, bool* flag)
{
    for (int i = 0; i <= filesize; i++)
	{
        if (buffer[i] == ' ' && buffer[i + 1] == ' ')
		{
			for (int j = i; buffer[j] != '\0'; j++)
            {
                buffer[j] = buffer[j + 1];
            }
            *flag = true;
        }
    }
}