#include "Differenciator.h"
#include "lexical_analysis.h"
#include "Reading.h"
#include "sys/stat.h"

List<node_t> ListScan(const char* filename);
size_t Filesize  (FILE *stream);

int main()
{
	List<node_t> lst = ListScan("main.amd");
	lst.GraphDump();
	DifferTree tree(GetGrammar(lst));
	tree.GraphDump();
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
		if (buffer[i] == '\r' || buffer[i] == '\n')
		{
			buffer[i] = ' ';
			for (int j = i; buffer[j + 1] != '\0'; j++)
			{
				buffer[j + 1] = buffer[j + 2];
			}
		}
	}
    return AnalysProcessing(buffer, filesize + 1);
}