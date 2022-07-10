#include "Compiliter.h"


void TranslateToAsm(List<DifferTree>& proga)
{
    char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");
    
    WritePreamble(fasm, proga);

    TranslateProcessing(fasm, proga, MainFuncName(proga));

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
    PrintConstant(fasm, proga);
    fputs("doublestr: db '\%lg \%lg', 0x0\n", fasm);

    fputs("section .text\n", fasm);
    fputs("\%define ", fasm);
    fputs(MainFuncName(proga), fasm);
    fputs(" main\n", fasm);
}

void PrintConstant(FILE* fasm, List<DifferTree> proga)
{       
    int size = proga.Size();

    for (int i = 0; i < size; i++)
    {
        DifferTree tree = proga.ShowFront();
        SearchConst(fasm, tree.Root());
        proga.PopFront();
    }
}



void SearchConst(FILE* fasm, node_t* node)
{
    static int num_const = 0;
    if (node->GetLeft()) SearchConst(fasm, node->GetLeft());
    if (node->dType() == DataType::CONSTANT)
    {
        fprintf(fasm, "const_%d: dq %lf\n", num_const++, node->value().number);
    }
    if (node->GetRight()) SearchConst(fasm, node->GetRight());
}



void TranslateProcessing(FILE* fasm, List<DifferTree>& proga, char* programm_name)
{
    int size = proga.Size();
    DifferTree function;

    for (int i = 0; i < size; i++)
    {
        function = proga.ShowFront();
        fputc('\n', fasm);
        TreeTranslate(fasm, function, programm_name);
        proga.PopFront();
    }
}


void TreeTranslate(FILE* fasm, DifferTree& function, char* programm_name)
{
    VerifyDefFunc(function);
    PrintProlog(fasm, function);

    node_t* func = function.ShowCurrent();

    if (strcmp(programm_name, NodeName(func)) == 0)
    {
        fputs("\t\tfinit\n", fasm);
    }

    TranslateOpSequence(fasm, func);

    PrintEpilog(fasm);
}


void PrintEpilog(FILE* fasm)
{
    fputs("\t\tleave\n", fasm);
    fputs("\t\tret\n", fasm);
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
    case DataType::MOV:
        TranslateMov(fasm, node, funcname);
        break;
    case DataType::IF:
        TranslateIf(fasm, node, funcname);
        break;
    default:
        break;
    }
}


void TranslateIf(FILE* fasm, node_t* node, char* funcname)
{

}



void TranslateInit(FILE* fasm, node_t* node, char* funcname)
{
    node = node->GetRight();
    static char* last_call_func = funcname;
    static int offset = 8;

    if (last_call_func != funcname)
    {
        offset = 8;
    }
    if (node->dType() == DataType::VARIABLE)
    {
        fprintf(fasm, "%%define %s_%s qword [rbp - %d]\n", funcname, NodeName(node), offset);

    }
    else if (node->dType() == DataType::MOV)
    {
        fprintf(fasm, "%%define %s_%s qword [rbp - %d]\n", funcname, NodeName(node->GetLeft()), offset);
        TranslateMov(fasm, node, funcname);
    }
    offset += 8;
    last_call_func = funcname;
}


void TranslateMov(FILE* fasm, node_t* node, char* funcname)
{
    TranslateExp(fasm, node->GetRight(), funcname);
    fprintf(fasm, "\t\tfstp\t%s_%s\n", funcname, NodeName(node->GetLeft()));
}


void TranslateExp(FILE* fasm, node_t* node, char* funcname)
{
    switch (node->dType())
    {
    case DataType::VARIABLE:
        fprintf(fasm, "\t\tfld\t\t%s_%s\n", funcname, NodeName(node));
        break;;
    case DataType::ADD:
        TranslateExp(fasm, node->GetLeft(), funcname);
        TranslateExp(fasm, node->GetRight(), funcname);
        fprintf(fasm, "\t\tfaddp\t\tst1\n");
        break;
    case DataType::SUB:
        TranslateExp(fasm, node->GetLeft(), funcname);
        TranslateExp(fasm, node->GetRight(), funcname);
        fprintf(fasm, "\t\tfsubp\t\tst1\n");
        break;
    case DataType::MUL:
        TranslateExp(fasm, node->GetLeft(), funcname);
        TranslateExp(fasm, node->GetRight(), funcname);
        fprintf(fasm, "\t\tfmulp\t\tst1\n");
        break;
    case DataType::DIV:
        TranslateExp(fasm, node->GetLeft(), funcname);
        TranslateExp(fasm, node->GetRight(), funcname);
        fprintf(fasm, "\t\tfdivp\t\tst1\n");
        break;
    case DataType::CONSTANT:
        static int num_const = 0;
        fprintf(fasm, "\t\tfld\t\tqword [const_%d]\n", num_const++);
        break;
    default:
        break;
    }
}


void TranslateCallFunc(FILE* fasm, node_t* node, char* funcname)
{
    char* call_func = NodeName(node);
    node = node->GetRight();
    int number_op = 0;
    if (node)
    {
        while (node->dType() == DataType::COMMA)
        {                    
            fprintf(fasm, "\t\tpush\t%s_%s\n", funcname, NodeName(node->GetRight()));
            node = node->GetLeft();
            number_op++;
        }
        fprintf(fasm, "\t\tpush\t%s_%s\n", funcname, NodeName(node));
        number_op++;
    }
    fprintf(fasm, "\t\tcall\t%s\n", call_func);
    fprintf(fasm, "\t\tadd\t\trsp, %d\n", number_op * 8);
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