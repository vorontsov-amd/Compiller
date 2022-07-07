#include "Compiliter.h"


void TranslateToAsm(List<DifferTree>& proga)
{
    char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");
    
    WritePreamble(fasm, proga);

    TranslateProcessing(fasm, proga);

    fclose(fasm);
}

char* ProgrammName(List<DifferTree>& proga)
{
    char* name = MainFuncName(proga);
    char* fullname = new char[strlen(name) + 5];
    strcpy(fullname, name);
    strcat(fullname, ".asm");
    return fullname;
}


char* MainFuncName(List<DifferTree>& proga)
{
    DifferTree main_func = proga.ShowFront();
    main_func.UpdateCurrent(answer::right);
    char* name = main_func.ShowCurrent()->value().string_ptr;
    return name;
}


void WritePreamble(FILE* fasm, List<DifferTree>& proga)
{
    fputs("global main\n", fasm);
    fputs("extern printf, scanf\n\n", fasm);
    fputs("section .data\n", fasm);
    fputs("doublestr: db '\%lg', 0x0\n\n", fasm);
    fputs("section .text\n", fasm);
    fputs("\%define ", fasm);
    fputs(MainFuncName(proga), fasm);
    fputs(" main\n", fasm);
}

void TranslateProcessing(FILE* fasm, List<DifferTree>& proga)
{
    int size = proga.Size();
    DifferTree function;

    for (int i = 0; i < size; i++)
    {
        function = proga.ShowFront();
        fputc('\n', fasm);
        TreeTranslate(fasm, function);
        proga.PopFront();
    }
}


void TreeTranslate(FILE* fasm, DifferTree& function)
{
    VerifyFunc(function);

    char* mark = FuncName(function);
    fputs(mark, fasm);
    fputs(":\n", fasm);
    fputs("\t\tpush\trbp\n", fasm);
    fputs("\t\tmov\t\trbp, rsp\n", fasm);

}

void VerifyFunc(DifferTree& function)
{
    if (function.ShowCurrent()->dType() != DataType::DEFINE)
    {
        std::cout << *function.ShowCurrent() << "\n";
        fprintf(stderr, "Error reading the function tree\n");
    }
    else 
    {
        function.UpdateCurrent(answer::right);
    }
}

inline char* FuncName(DifferTree& function)
{
    return function.ShowCurrent()->value().string_ptr;
}