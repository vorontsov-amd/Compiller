#include "Compiliter.h"



void TranslateToAsm(List<DifferTree>& proga)
{
    char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");

    ByteArray machine_code;
    
    WritePreamble(fasm, proga);

    TranslateProcessing(fasm, proga, MainFuncName(proga), machine_code);

    FILE* out = fopen("out", "wb");
    
    fwrite(machine_code.ByteCode(), 1, machine_code.Size(), out);
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
    fputs("extern printf, scanf, putchar\n\n", fasm);

    fputs("section .data\n", fasm);
    PrintConstant(fasm, proga);
    fputs("print_double: db '\%lg ', 0x0\n", fasm);
    fputs("scan_double: db '\%lg', 0x0\n", fasm);

    fputs("section .bss\n", fasm);
    fputs("result: resq 1\n", fasm);

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



void TranslateProcessing(FILE* fasm, List<DifferTree>& proga, char* programm_name, ByteArray& machine_code)
{
    int size = proga.Size();
    DifferTree function;

    for (int i = 0; i < size; i++)
    {
        function = proga.ShowFront();
        fputc('\n', fasm);
        TreeTranslate(fasm, function, programm_name, machine_code);
        proga.PopFront();
    }
}


void TreeTranslate(FILE* fasm, DifferTree& function, char* programm_name, ByteArray& machine_code)
{
    VerifyDefFunc(function);
    List<variable>* param = PrintProlog(fasm, function, machine_code);
    int offset = param->Size() * 8;

    node_t* func = function.ShowCurrent();

    if (strcmp(programm_name, NodeName(func)) == 0)
    {
        fputs("\t\tfinit\n", fasm);
        machine_code.Append(0x9b);
        machine_code.Append(0xdb);
        machine_code.Append(0xe3);
    }

    char* funcname = NodeName(func);
    TranslateOpSequence(fasm, param, func->GetRight(), funcname, offset);

    node_t null(NodeType::NUMBER, DataType::CONSTANT, 0.0);
    PrintEpilog(fasm, param, &null, funcname);
}


void PrintEpilog(FILE* fasm, List<variable>* param, node_t* node, char* funcname)
{
    fprintf(fasm, ".ret_%s:\n", funcname);
    
    for (int i = 0, size = param->Size(); i < size; i++)
    {
        variable var = param->ShowFront();
        if (var.IsLink())
        {
            fprintf(fasm, "\t\tmov\t\trax, [rbp + %ld]\n", var.Offset() + 8);
            fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp - %ld]\n", var.Offset());
            fprintf(fasm, "\t\tmovsd\tqword [rax], xmm0\n");
        }
        param->PopFront();
    }

    fputs("\t\tleave\n", fasm);
    fputs("\t\tret\n", fasm);
}


void TranslateOpSequence(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset)
{
    while (node && (node->dType() == DataType::END_OP))
    {
        TranslateOp(fasm, param, node->GetLeft(), funcname, offset);
        node = node->GetRight();
    }
}

void TranslateOp(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset)
{
    switch (node->dType())
    {
    case DataType::FUNC:
        TranslateCallFunc(fasm, param, node, funcname);
        break;
    case DataType::PRINTF:
        TranslateCallPrintf(fasm, param, node, funcname);
        break;
    case DataType::SCANF:
        TranslateCallScanf(fasm, param, node, funcname);
        break;
    case DataType::SQRT:
        TransateCallSqtr(fasm, param, node, funcname);
        break;
    case DataType::INITIALIZATE:
        TranslateInit(fasm, param, node, funcname, offset);
        break;
    case DataType::MOV:
        TranslateMov(fasm, param, node, funcname);
        break;
    case DataType::IF:
        TranslateIf(fasm, param, node, funcname, offset);
        break;
    case DataType::WHILE:
        TranslateWhile(fasm, param, node, funcname, offset);
        break;
    case DataType::RET:
        TranslateRet(fasm, param, node, funcname);
        break;
    default:
        break;
    }
}


void TransateCallSqtr(FILE* fasm, List<variable>* param, node_t* node, char* funcname)
{
    node = node->GetRight();
    TranslateExp(fasm, param, node, funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tfld\tqword [result]\n");
    fprintf(fasm, "\t\tfsqrt\n");
    fprintf(fasm, "\t\tfstp\tqword [result]\n");
    fprintf(fasm, "\t\tpush\tqword [result]\n");
}



void TranslateCallScanf(FILE* fasm, List<variable>* lst, node_t* node, char* funcname)
{
    node = node->GetRight();

    List<node_t*> StackNodePtr; 
    while (node && (node->dType() == DataType::COMMA))
    {
        StackNodePtr.PushBack(node);
        node = node->GetLeft();
    }

    fprintf(fasm, "\t\tmov\t\trdi, scan_double\n");
    fprintf(fasm, "\t\tlea\t\trsi, [rbp - %ld]\n", OffsetVariable(lst, node));
    fprintf(fasm, "\t\tmov\t\teax, 1\n");
    fprintf(fasm, "\t\tcall\tscanf\n");

    for (int i = 0, size = StackNodePtr.Size(); i < size; i++)
    {
        node_t* node = StackNodePtr.ShowBack();
        fprintf(fasm, "\t\tmov\t\trdi, scan_double\n");
        fprintf(fasm, "\t\tlea\t\trsi, [rbp - %ld]\n", OffsetVariable(lst, node->GetRight()));
        fprintf(fasm, "\t\tmov\t\teax, 1\n");
        fprintf(fasm, "\t\tcall\tscanf\n");
        StackNodePtr.PopBack();
    }
}




uint64_t OffsetVariable(List<variable>* param, node_t* var_ptr)
{
    List<variable>* lst = new List<variable>(*param);
    
    for (int i = 0, size = lst->Size(); i < size; i++)
    {
        if (strcmp(lst->ShowFront().Name(), var_ptr->Name()) == 0)
        {
            return lst->ShowFront().Offset();
        }
        lst->PopFront();
    }

    delete lst;
    return 0;
}






void TranslateRet(FILE* fasm, List<variable>* param, node_t* node, char* funcname)
{
    node = node->GetRight();
    TranslateExp(fasm, param, node, funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
    fprintf(fasm, "\t\tjmp\t\t.ret_%s\n", funcname);
}



void TranslateCallPrintf(FILE* fasm, List<variable>* param, node_t* node, char* funcname)
{
    node = node->GetRight();

    List<node_t*> StackNodePtr; 
    while (node && (node->dType() == DataType::COMMA))
    {
        StackNodePtr.PushBack(node);
        node = node->GetLeft();
    }

    fprintf(fasm, "\t\tmov\t\trdi, print_double\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp - %ld]\n", OffsetVariable(param, node));
    fprintf(fasm, "\t\tmov\t\teax, 1\n");
    fprintf(fasm, "\t\tcall\tprintf\n");

    for (int i = 0, size = StackNodePtr.Size(); i < size; i++)
    {
        node_t* node = StackNodePtr.ShowBack();
        fprintf(fasm, "\t\tmov\t\trdi, print_double\n");
        fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp - %ld]\n", OffsetVariable(param, node->GetRight()));
        fprintf(fasm, "\t\tmov\t\teax, 1\n");
        fprintf(fasm, "\t\tcall\tprintf\n");
        StackNodePtr.PopBack();
    }

    fprintf(fasm, "\t\tmov\t\trdi, 10d\n");
    fprintf(fasm, "\t\tcall\tputchar\n");
}


void TranslateWhile(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset)
{
    static int num_while = 0;
    node_t* condition = node->GetLeft();

    fprintf(fasm, "\t\tjmp\t\t.while%dtest\n", num_while);
    fprintf(fasm, ".while%dloop:\n", num_while);
    TranslateOpSequence(fasm, param, node->GetRight(), funcname, offset);

    fprintf(fasm, ".while%dtest:\n", num_while);
    TranslateExp(fasm, param, condition->GetLeft(), funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
    TranslateExp(fasm, param, condition->GetRight(), funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tmovsd\txmm1, qword [result]\n");
    fprintf(fasm, "\t\tcomisd\txmm0, xmm1\n");
    fprintf(fasm, "\t\t%s\t\t.while%dloop\n", Jxx(condition), num_while++);
}


void TranslateIf(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int offset)
{
    static int num_if = -1;
    num_if++;
    int save_num = num_if;

    node_t* condition = node->GetLeft();
    TranslateExp(fasm, param, condition->GetLeft(), funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
    TranslateExp(fasm, param, condition->GetRight(), funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tmovsd\txmm1, qword [result]\n");
    fprintf(fasm, "\t\tcomisd\txmm0, xmm1\n");

    if (node->GetRight()->dType() == DataType::ELSE)
    {
        fprintf(fasm, "\t\t%s\t\t.if%delse\n", Jnx(condition), save_num);
        node_t* op_else = node->GetRight();
        TranslateOpSequence(fasm, param, op_else->GetLeft(), funcname, offset);
        fprintf(fasm, "\t\tjmp\t\t.if%dend\n", save_num);
        fprintf(fasm, ".if%delse:\n", save_num);
        if (op_else->GetRight()->dType() != DataType::IF)
        {
            TranslateOpSequence(fasm, param, op_else->GetRight(), funcname, offset);
        }
        else
        {
            TranslateIf(fasm, param, op_else->GetRight(), funcname, offset);
        }
        fprintf(fasm, ".if%dend:\n", save_num);
    }
    else
    {
        fprintf(fasm, "\t\t%s\t\t.if%dend\n", Jnx(condition), num_if);
        TranslateOpSequence(fasm, param, node->GetRight(), funcname, offset);
        fprintf(fasm, ".if%dend:\n", save_num);
    }
}


const char* Jnx(node_t* node)
{
    char* jxx = nullptr;   
    switch (node->dType())
    {
    case DataType::JE:
        return "jne";
    case DataType::JNE:
        return "je";
    case DataType::JA:
        return "jbe";
    case DataType::JAE:
        return "jb";
    case DataType::JB:
        return "jae";
    case DataType::JBE:
        return "ja";
    default:
        fprintf(stderr, "Error in condition. Data type isn't condition");
        break;
    }
    return nullptr;
}


const char* Jxx(node_t* node)
{
    char* jxx = nullptr;   
    switch (node->dType())
    {
    case DataType::JE:
        return "je";
    case DataType::JNE:
        return "jne";
    case DataType::JA:
        return "ja";
    case DataType::JAE:
        return "jae";
    case DataType::JB:
        return "jb";
    case DataType::JBE:
        return "jbe";
    default:
        fprintf(stderr, "Error in condition. Data type isn't condition");
        break;
    }
    return nullptr;
}



void TranslateInit(FILE* fasm, List<variable>* param, node_t* node, char* funcname, int initial_offset)
{
    node = node->GetRight();
    static char* last_call_func = funcname;
    static int offset = initial_offset + 8;

    if (last_call_func != funcname)
    {
        offset = initial_offset + 8;
    }
    if (node->dType() == DataType::VARIABLE)
    {
        variable var(node, false, offset);
        param->PushFront(var);
        //fprintf(fasm, "%%define %s_%s [rbp - %d]\n", funcname, NodeName(node), offset);

    }
    else if (node->dType() == DataType::MOV)
    {
        variable var(node->GetLeft(), false, offset);
        param->PushFront(var);        
        //fprintf(fasm, "%%define %s_%s [rbp - %d]\n", funcname, NodeName(node->GetLeft()), offset);
        TranslateMov(fasm, param, node, funcname);
    }
    offset += 8;
    last_call_func = funcname;
}


void TranslateMov(FILE* fasm, List<variable>* param, node_t* node, char* funcname)
{
    TranslateExp(fasm, param, node->GetRight(), funcname);
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
    fprintf(fasm, "\t\tmovsd\tqword [rbp - %ld], xmm0\n", OffsetVariable(param, node->GetLeft()));
}


void TranslateExp(FILE* fasm, List<variable>* param, node_t* node, char* funcname, bool st_exp)
{
    bool start_expr = st_exp;
    
    switch (node->dType())
    {
    case DataType::VARIABLE:
        fprintf(fasm, "\t\tpush\tqword [rbp - %ld]\n", OffsetVariable(param, node));
        break;;
    case DataType::ADD:
        TranslateExp(fasm, param, node->GetLeft(), funcname, false);
        TranslateExp(fasm, param, node->GetRight(), funcname, false);
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm1, [result]\n");
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
        fprintf(fasm, "\t\taddsd\txmm0, xmm1\n");
        fprintf(fasm, "\t\tmovsd\tqword [result], xmm0\n");
        fprintf(fasm, "\t\tpush\tqword [result]\n");
        break;
    case DataType::SUB:
        if (node->GetLeft())
        {
            TranslateExp(fasm, param, node->GetLeft(), funcname, false);
        }
        else
        {
            fprintf(fasm, "\t\tfldz\n");
            fprintf(fasm, "\t\tfstp\tqword [result]\n");
            fprintf(fasm, "\t\tpush\tqword [result]\n");
        }
        TranslateExp(fasm, param, node->GetRight(), funcname, false);
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm1, [result]\n");
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
        fprintf(fasm, "\t\tsubsd\txmm0, xmm1\n");
        fprintf(fasm, "\t\tmovsd\tqword [result], xmm0\n");
        fprintf(fasm, "\t\tpush\tqword [result]\n");
        break;
    case DataType::MUL:
        TranslateExp(fasm, param, node->GetLeft(), funcname, false);
        TranslateExp(fasm, param, node->GetRight(), funcname, false);
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm1, qword [result]\n");
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
        fprintf(fasm, "\t\tmulsd\txmm0, xmm1\n");
        fprintf(fasm, "\t\tmovsd\tqword [result], xmm0\n");
        fprintf(fasm, "\t\tpush\tqword [result]\n");
        break;
    case DataType::DIV:
        TranslateExp(fasm, param, node->GetLeft(), funcname, false);
        TranslateExp(fasm, param, node->GetRight(), funcname, false);
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm1, [result]\n");
        fprintf(fasm, "\t\tpop\t\tqword [result]\n");
        fprintf(fasm, "\t\tmovsd\txmm0, qword [result]\n");
        fprintf(fasm, "\t\tdivsd\txmm0, xmm1\n");
        fprintf(fasm, "\t\tmovsd\tqword [result], xmm0\n");
        fprintf(fasm, "\t\tpush\tqword [result]\n");
        break;
    case DataType::CONSTANT:
        static int num_const = 0;
        fprintf(fasm, "\t\tpush\tqword [const_%d]\n", num_const++);
        break;
    case DataType::FUNC:
        TranslateCallFunc(fasm, param, node, funcname);
        fprintf(fasm, "\t\tmovsd\tqword [result], xmm0\n");
        fprintf(fasm, "\t\tpush\tqword [result]\n");
        break;
    case DataType::SQRT:
        TransateCallSqtr(fasm, param, node, funcname);
        break;   
    default:
        break;
    }
}


void TranslateCallFunc(FILE* fasm, List<variable>* lst, node_t* node, char* funcname)
{
    char* call_func = NodeName(node);
    node = node->GetRight();
    int number_op = 0;
    if (node)
    {
        while (node->dType() == DataType::COMMA)
        {                    
            fprintf(fasm, "\t\tlea\t\trax, [rbp - %ld]\n", OffsetVariable(lst, node->GetRight()));
            fprintf(fasm, "\t\tpush\trax\n");
            node = node->GetLeft();
            number_op++;
        }
        fprintf(fasm, "\t\tlea\t\trax, [rbp - %ld]\n", OffsetVariable(lst, node));
        fprintf(fasm, "\t\tpush\trax\n");
        number_op++;
    }
    fprintf(fasm, "\t\tcall\t%s\n", call_func);
    fprintf(fasm, "\t\tadd\t\trsp, %d\n", number_op * 8);
}


void SaveFPU(FILE* fasm)
{
    fprintf(fasm, "\t\tfstp\t\tqword [result]\n");
    fprintf(fasm, "\t\tpush\tqword [result]\n");
}


void LoadFPU(FILE* fasm)
{
    fprintf(fasm, "\t\tpop\t\tqword [result]\n");
    fprintf(fasm, "\t\tfld\t\tqword [result]\n");
}






List<variable>* PrintProlog(FILE* fasm, DifferTree& function, ByteArray& machine_code)
{
    node_t* func_node = function.ShowCurrent();
    
    char* mark = NodeName(func_node);
    fputs(mark, fasm);
    fputs(":\n", fasm);
    
    List<variable>* variables = PrintDefineParam(fasm, func_node);

    fputs("\t\tpush\trbp\n", fasm);
    machine_code.Append(0x55);
    fputs("\t\tmov\t\trbp, rsp\n", fasm);
    machine_code.Append(0x48);
    machine_code.Append(0x89);
    machine_code.Append(0xe5);

    int num_param = 0 ;
    NumVar(num_param, func_node);
    num_param += variables->Size();

    uint64_t offset = 0;
    if (num_param % 2 == 0)
    {
        offset = num_param * 8;
    }
    else
    {
        offset = (num_param + 1) * 8;
    }
    fprintf(fasm, "\t\tsub\t\trsp, %ld\n", offset);
    TTHex_sub_rsp_number(offset, machine_code);


    List<variable>* lst = new List<variable>(*variables);
    for (int i = 0, size = lst->Size(); i < size; i++)
    {
        variable var = lst->ShowFront();
        fprintf(fasm, "\t\tmov\t\trax, [rbp + %ld]\n", var.Offset() + 8);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [rax]\n");
        fprintf(fasm, "\t\tmovsd\t[rbp - %ld], xmm0\n", var.Offset());
        lst->PopFront();
    }

    delete lst;
    return variables;
}


void TTHex_sub_rsp_number(unsigned long long offset, ByteArray& machine_code)
{
    if (offset < 0x80)
    {
        machine_code.Append(0x48);
        machine_code.Append(0x83);
        machine_code.Append(0xec);
        machine_code.Append(offset);
    }
    else
    {
        machine_code.Append(0x48);
        machine_code.Append(0x81);
        machine_code.Append(0xec);
        for (int i = 0; i < 4; i++)
        {
            int byte = offset % 0x100;
            machine_code.Append(byte);
            offset = offset / 0x100;
        }
    }
}


List<variable>* PrintDefineParam(FILE* fasm, node_t* node)
{
    VerifyFunc(node);
    char* funcname = NodeName(node);
    List<variable>* variables = new List<variable>;

    if (node->GetLeft())
    {
        int offset = NumParam(node) * 8;
        node = node->GetLeft();
        while (node->dType() == DataType::COMMA)
        {
            node_t* var_ptr = node->GetRight();
            if (var_ptr->dType() == DataType::NEW_VAR)
            {
                var_ptr = var_ptr->GetRight();
                variable var(var_ptr, false, offset);
                variables->PushBack(var);
            }
            else
            {
                variable var(var_ptr, true, offset);
                variables->PushBack(var);
            }
            // fprintf(fasm, "%%define %s_%s_ptr [rbp + %d]\n", funcname, NodeName(var_ptr), offset + 8);
            // fprintf(fasm, "%%define %s_%s [rbp - %d]\n", funcname, NodeName(var_ptr), offset);
            offset -= 8;
            node = node->GetLeft();
        }
        if (node->dType() == DataType::NEW_VAR)
        {
            node = node->GetRight();
            variable var(node, false, offset);
            variables->PushBack(var);
        }
        else
        {
            variable var(node, true, offset);
            variables->PushBack(var);
        }
        // fprintf(fasm, "%%define %s_%s_ptr [rbp + %d]\n", funcname, NodeName(node), offset + 8);
        // fprintf(fasm, "%%define %s_%s [rbp - %d]\n", funcname, NodeName(node), offset);
    }

    return variables;
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