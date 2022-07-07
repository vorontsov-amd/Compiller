#include "Compiliter.h"


void TranslateToAsm(List<DifferTree>& proga)
{
    LOX

    char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");
    
    LOX
    
    WritePreamble(fasm, proga);

    LOX

    TranslateProcessing(proga);

    fclose(fasm);
}

char* ProgrammName(List<DifferTree>& proga)
{
    LOX
    char* name = MainFuncName(proga);
    LOX
    char* fullname = new char[strlen(name) + 5];
    strcpy(fullname, name);
    strcat(fullname, ".asm");
    return fullname;
}


char* MainFuncName(List<DifferTree>& proga)
{
    LOX    
    DifferTree main_func = proga.ShowFront();
    LOX
    main_func.UpdateCurrent(answer::right);
    LOX
    char* name = proga.ShowFront().ShowCurrent().value().string_ptr;
    LOX
    proga.ShowFront().UpdateCurrent(answer::root);
    LOX
    return name;
}


void WritePreamble(FILE* fasm, List<DifferTree>& proga)
{
    fputs("global main\n", fasm);
    fputs("extern printf, scanf\n", fasm);
    fputs("section .data\n", fasm);
    fputs("doublestr: db '\%lg', 0x0\n", fasm);
    fputs("section .text\n", fasm);
    fputs("\%define ", fasm);
    fputs(MainFuncName(proga), fasm);
    fputs(" main", fasm);
}

void TranslateProcessing(List<DifferTree>& proga)
{
    int size = proga.Size();
    DifferTree function;

    for (int i = 0; i < size; i++)
    {
        function = proga.ShowFront();
        TreeTranslate(function);
        proga.PopFront();
    }
}


void TreeTranslate(DifferTree& function)
{
    VerifyFunc(function);

    
}

void VerifyFunc(DifferTree& function)
{
    if (function.ShowCurrent().dType() != DataType::DEFINE)
    {
        fprintf(stderr, "Error reading the function tree\n");
    }
    else 
    {
        function.UpdateCurrent(answer::right);
    }
}