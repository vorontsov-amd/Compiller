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
void VerifyMainArgument(int argc, const char* argv[]);

int main(int argc, char const *argv[])
{    
    VerifyMainArgument(argc, argv);
    
    const char* filename_code = argv[1];
    const char* filename_text = argv[2];

    List<node_t> lst = ListScan(filename_text);
	lst.GraphDump();

	List<DifferTree> tree = GetGrammar(lst);
    ProgrammDump(tree);

    TranslateToAsm(tree, filename_code);
}

void VerifyMainArgument(int argc, const char* argv[])
{
	switch (argc)
    {
    case 1:
        puts("file for compillig not detected");
        exit(EXIT_SUCCESS);
    case 2:
        puts("you have to enter 2 arguments");
        exit(EXIT_SUCCESS);
    case 3: 
        return;
    default:
        puts("compiliter can compillite only one file");
        exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[0], argv[1]) == 0)
    {
        puts("you can't name a out file with a compiler name");
        exit(EXIT_SUCCESS);
    }
}

void ProgrammDump(List<DifferTree> tree)
{
    int size = tree.Size();

    for (int i = 0; i < size; i++)
    {
        const char* funcname = MainFuncName(tree);
        tree.ShowFront().GraphDump(funcname);
        tree.PopFront();
    }
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