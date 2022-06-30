#include "Differenciator.h"
#include "lexical_analysis.h"
#include "Reading.h"
#include "sys/stat.h"
#include "stdlib.h"

List<node_t> ListScan(const char* filename);
size_t Filesize  (FILE *stream);
void ClearBuf(char* buffer, size_t filesize, bool* flag);
void ProgrammDump(List<DifferTree>& tree);
char* NameFunc(DifferTree tree);

int main()
{
	List<node_t> lst = ListScan("main.amd");
	lst.GraphDump();
	List<DifferTree> tree = GetGrammar(lst);  
    ProgrammDump(tree);  
}

void ProgrammDump(List<DifferTree>& tree)
{
    int size = tree.Size();

    for (int i = 0; i < size; i++)
    {
        char* func = NameFunc(tree.ShowFront());
        tree.ShowFront().GraphDump(func);
        tree.PopFront();
    }
}

char* NameFunc(DifferTree tree)
{
    tree.UpdateCurrent(answer::right);
    char* str = tree.ShowCurrent().value().string_ptr;
    tree.UpdateCurrent(answer::root);
    return str;
}


size_t Filesize  (FILE *stream)  
{
    struct stat buff = {};

    fstat(fileno(stream), &buff);
    
    return buff.st_size;
}

List<node_t> ListScan(const char* filename)
{
    FILE* stream = fopen("main.amd", "r");
	
	size_t filesize = Filesize(stream);

    char* buffer = (char*) calloc(filesize + 1, sizeof(char));
    if (buffer == nullptr)
    {
        fclose(stream);
    }

    size_t count = fread(buffer, sizeof(char), filesize, stream);
    if (count != filesize)
    {
        fclose(stream);
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