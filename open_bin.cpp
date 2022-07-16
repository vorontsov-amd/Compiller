#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
    int dataSize = 0;
    FILE* fp;

    fp = fopen("file.b", "rb");

    fseek(fp, 0, SEEK_END);
    dataSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    printf("Data Size %d\n", dataSize);

    char* binary_code;
    binary_code = (char*)malloc(dataSize);
    fread(binary_code, dataSize, 1, fp);
    fclose(fp);

    puts("1");

    void* mem = mmap(NULL, dataSize, PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    memcpy(mem, binary_code, dataSize);

    puts("2");

    int (*runCodeFunc) (int, int) = (int (*)(int, int))mem;

    puts("3");

    printf("Run... (%d, %d) result is %d\n", 7, 5, runCodeFunc(7,5));

    puts("4");

    free(binary_code);
    munmap(mem, dataSize);
}