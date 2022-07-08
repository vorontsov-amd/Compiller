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
    VerifyDefFunc(function);
    PrintProlog(fasm, function);

    node_t* func = function.ShowCurrent();

    TranslateOpSequence(fasm, func);
}


void TranslateOpSequence(FILE* fasm, node_t* node)
{
    VerifyFunc(node);
    char* funcname = NodeName(node);
    node = node->GetRight();

    while (node && (node->dType() == DataType::END_OP))
    {
        TranslateOp(fasm, node->GetLeft(), funcname);
        node = node->GetRight();
    }
}

void TranslateOp(FILE* fasm, node_t* node, char* funcname)
{
    switch (node->dType())
    {
    case DataType::FUNC:
        TranslateCallFunc(fasm, node, funcname);
        break;
    case DataType::INITIALIZATE:
        TranslateInit(fasm, node, funcname);
        break;
    default:
        break;
    }
}

void TranslateInit(FILE* fasm, node_t* node, char* funcname)
{
    node = node->GetRight();
    static char* namefunc = funcname;
    static int offset = 8;

    if (namefunc != funcname)
    {
        offset = 8;
    }
    if (node->dType() == DataType::VARIABLE)
    {
        fprintf(fasm, "%%define %s_%s qword [rbp - %d]\n", funcname, NodeName(node), offset);
        if (namefunc == funcname) offset += 8;
        namefunc = funcname;
    }
}


void TranslateCallFunc(FILE* fasm, node_t* node, char* funcname)
{
    char* call_func = NodeName(node);
    node = node->GetRight();
    if (node)
    {
        while (node->dType() == DataType::COMMA)
        {                    
            fprintf(fasm, "\t\tpush\t%s_%s\n", funcname, NodeName(node->GetRight()));
            node = node->GetLeft();
        }
        fprintf(fasm, "\t\tpush\t%s_%s\n", funcname, NodeName(node));
    }
    fprintf(fasm, "\t\tcall\t%s\n", call_func);
}




void PrintProlog(FILE* fasm, DifferTree& function)
{
    node_t* func_node = function.ShowCurrent();
    
    char* mark = NodeName(func_node);
    fputs(mark, fasm);
    fputs(":\n", fasm);
    
    PrintDefineParam(fasm, func_node);

    fputs("\t\tpush\trbp\n", fasm);
    fputs("\t\tmov\t\trbp, rsp\n", fasm);

    int num_param = 0 ;
    NumVar(num_param, func_node);
    if (num_param % 2 == 0)
    {
        fprintf(fasm, "\t\tsub\t\trsp, %d\n", num_param * 8);
    }
    else
    {
        fprintf(fasm, "\t\tsub\t\trsp, %d\n", (num_param + 1) * 8);
    }
}

void PrintDefineParam(FILE* fasm, node_t* node)
{
    VerifyFunc(node);
    char* funcname = NodeName(node);

    if (node->GetLeft())
    {
        int offset = NumParam(node) * 8 + 8;
        node = node->GetLeft();
        while (node->dType() == DataType::COMMA)
        {
            fprintf(fasm, "%%define %s_%s qword [rbp + %d]\n", funcname, NodeName(node->GetRight()), offset);
            node = node->GetLeft();
            offset -= 8;
        }
        fprintf(fasm, "%%define %s_%s qword [rbp + %d]\n", funcname, NodeName(node), offset);
    }
}

int NumParam(node_t* node)
{
    int num_param = 0;
    while (node->GetLeft())
    {
        num_param++;
        node = node->GetLeft();
    }
    return num_param;
}



void NumVar(int& num_param, node_t* node)
{    
    if (node->dType() == DataType::INITIALIZATE)
    {
        num_param++;
        return;
    }
    if (node->GetLeft()) NumVar(num_param, node->GetLeft());
    if (node->GetRight()) NumVar(num_param, node->GetRight());
}

void VerifyFunc(node_t* node)
{
    if (node->dType() != DataType::FUNC)
    {
        std::cout << node << "\n";
        fprintf(stderr, "Error reading function parametrs");
    }
}


void VerifyDefFunc(DifferTree& function)
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

inline char* NodeName(node_t* function)
{
    return function->value().string_ptr;
}