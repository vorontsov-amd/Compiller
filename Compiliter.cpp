#include "Compiliter.h"


void TranslateToAsm(List<DifferTree>& proga)
{
    const char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");

    ByteArray machine_code;
    Stubs stubs;
    List<Label>* lbl = new List<Label>;
    stubs.labels = *lbl;

    WriteELFHeader(stubs, machine_code);
    WritePreamble(fasm, proga, stubs, machine_code);
    TranslateProcessing(fasm, proga, stubs, machine_code);
    LOX
    LOX

    stubs.is_loading = false;
    stubs.rewind_const = true;
    stubs.rewind_if = true;
    stubs.rewind_while = true;
    stubs.rewind_init = true;
    stubs.rewind_const_expr = true;
    rewind(fasm);
    machine_code.Rewind();

    LOX

    WriteELFHeader(stubs, machine_code);

    LOX

    WritePreamble(fasm, proga, stubs, machine_code);

    LOX

    TranslateProcessing(fasm, proga, stubs, machine_code);

    LOX

    FILE* out = fopen("out", "wb");    
    fwrite(machine_code.ByteCode(), 1, machine_code.Size(), out);
    fclose(fasm);
}

void WriteELFHeader(Stubs& stubs, ByteArray& machine_code)
{
    const int NO_USE = 0;
    stubs.ElfStubs.e_entry = 0x30200000;

    Elf64_Ehdr elf_header = {
        .e_ident = {ELFMAG0,       ELFMAG1,     ELFMAG2,       ELFMAG3,
                    ELFCLASS64,    ELFDATA2LSB, EV_CURRENT,    ELFOSABI_NONE,
                    ELFOSABI_NONE, NO_USE,      NO_USE,        NO_USE,
                    NO_USE,        NO_USE,      NO_USE,        sizeof(elf_header.e_ident)},
        .e_type      = ET_EXEC,
        .e_machine   = EM_X86_64,
        .e_version   = EV_CURRENT,
        .e_entry     = stubs.ElfStubs.text_stubs.p_vaddp,
        .e_phoff     = sizeof(Elf64_Ehdr),
        .e_shoff     = NO_USE,
        .e_flags     = NO_USE,
        .e_ehsize    = sizeof(Elf64_Ehdr),
        .e_phentsize = sizeof(Elf64_Phdr),
        .e_phnum     = 3,
        .e_shentsize = NO_USE,
        .e_shnum     = NO_USE,
        .e_shstrndx  = NO_USE,
    };
    machine_code.Append(elf_header);

    Elf64_Phdr data_header = {
        .p_type    = PT_LOAD,
        .p_flags   = PF_R | PF_W,
        .p_offset  = stubs.ElfStubs.data_stubs.p_offset,
        .p_vaddr   = stubs.ElfStubs.data_stubs.p_vaddp,
        .p_paddr   = NO_USE,
        .p_filesz  = stubs.ElfStubs.data_stubs.p_size,
        .p_memsz   = stubs.ElfStubs.data_stubs.p_size,
        .p_align   = 0x1000,
    };
    machine_code.Append(data_header);

    Elf64_Phdr rodata_header = {
        .p_type    = PT_LOAD,
        .p_flags   = PF_R,
        .p_offset  = stubs.ElfStubs.rodata_stubs.p_offset,
        .p_vaddr   = stubs.ElfStubs.rodata_stubs.p_vaddp,
        .p_paddr   = NO_USE,
        .p_filesz  = stubs.ElfStubs.rodata_stubs.p_size,
        .p_memsz   = stubs.ElfStubs.rodata_stubs.p_size,
        .p_align   = 0x1000,
    };
    machine_code.Append(rodata_header);
        
    Elf64_Phdr text_header = {
        .p_type    = PT_LOAD,
        .p_flags   = PF_X | PF_R | PF_W,
        .p_offset  = stubs.ElfStubs.text_stubs.p_offset,
        .p_vaddr   = stubs.ElfStubs.text_stubs.p_vaddp,
        .p_paddr   = NO_USE,
        .p_filesz  = stubs.ElfStubs.text_stubs.p_size,
        .p_memsz   = stubs.ElfStubs.text_stubs.p_size,
        .p_align   = 0x1000,
    };
    machine_code.Append(text_header);
}


const char* ProgrammName(List<DifferTree>& proga)
{
    const char* name = MainFuncName(proga);
    char* fullname = new char[strlen(name) + 5];
    strcpy(fullname, name);
    strcat(fullname, ".asm");
    return fullname;
}

const char* MainFuncName(List<DifferTree>& proga)
{
    DifferTree main_function = proga.ShowFront();
    node_t function = main_function.Root()->GetRight();
    return function.Name();
}

void WritePreamble(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code)
{
    fputs("global main\n", fasm);
    fputs("extern printf, scanf, putchar\n\n", fasm);
    PreambleData(fasm, proga, stubs, machine_code);
    LOX
    PreambleRodata(fasm, proga, stubs, machine_code);
    LOX
    fputs("section .text\n", fasm);
}

void PreambleRodata(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code)
{
    fputs("section .rodata\n", fasm);
    uint64_t rodata_begin = machine_code.Size();
    if (stubs.is_loading)
    {
        stubs.ElfStubs.rodata_stubs.p_offset = rodata_begin;
        stubs.ElfStubs.rodata_stubs.p_vaddp = stubs.ElfStubs.e_entry + rodata_begin;
    }
    LOX
    WriteConstant(fasm, proga, stubs, machine_code);
    fprintf(fasm, "print_double: db '%%lg ', 0x0\n");
    LOX
    uint16_t rodata_end = machine_code.Size();
    if (stubs.is_loading)
    {
        stubs.ElfStubs.rodata_stubs.p_size = rodata_end - rodata_begin;
    }
}

void PreambleData(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code)
{
    fputs("section .data\n", fasm);
    if (stubs.is_loading)
    {
        stubs.ElfStubs.data_stubs.p_offset = machine_code.Size();
        stubs.ElfStubs.data_stubs.p_vaddp = stubs.ElfStubs.e_entry + machine_code.Size();
        stubs.ElfStubs.data_stubs.p_size = sizeof(double);
    }
    fputs("buffer: dq 0.0\n", fasm);
    machine_code.Append(0.0);
    fputs("str: dd 0\n", fasm);
    machine_code.Append(0U);
}

void WriteConstant(FILE* fasm, List<DifferTree> proga, Stubs& stubs, ByteArray& machine_code)
{       
    LOX
    int size = proga.Size();
    LOX
    for (int i = 0; i < size; i++)
    {
        LOX
        DifferTree tree = proga.ShowFront();
        SearchConst(fasm, tree.Root(), stubs, machine_code);
        proga.PopFront();
    }
}

// void StrToHex(const char* str, ByteArray& machine_code)
// {
//     for (int i = 0; str[i] != '\0'; i++)
//     {
//         machine_code.Append(str[i]);
//     }
//     machine_code.Append(0x00);
// }

void SearchConst(FILE* fasm, node_t* node, Stubs& stubs, ByteArray& machine_code)
{
    if (node->GetLeft()) SearchConst(fasm, node->GetLeft(), stubs, machine_code);
    if (node->dType() == DataType::CONSTANT)
    {
        AppendConst(fasm, node, stubs, machine_code);
    }
    if (node->GetRight()) SearchConst(fasm, node->GetRight(), stubs, machine_code);
}

void AppendConst(FILE* fasm, node_t* node, Stubs& stubs, ByteArray& machine_code)
{    
    static int num_const = 0;
    if (stubs.rewind_const)
    {
        num_const = 0;
        stubs.rewind_const = false;
    }
    
    double number = node->value().number;
    char str[15] = "";
    snprintf(str, 15, "const_%d", num_const);

    fprintf(fasm, "%s: dq %lf\n", str, number);

    addr_t addres = stubs.ElfStubs.rodata_stubs.p_vaddp + num_const * sizeof(double);
    Label new_label(str, addres);
    stubs.labels.PushBack(new_label);
    machine_code.Append(number);

    num_const++;
}

void TranslateProcessing(FILE* fasm, List<DifferTree>& proga, Stubs& stubs, ByteArray& machine_code)
{
    List<DifferTree> lst = proga;
    
    uint64_t begin_text = machine_code.Size();
    if (stubs.is_loading)
    {
        stubs.ElfStubs.text_stubs.p_offset = begin_text;
        stubs.ElfStubs.text_stubs.p_vaddp = stubs.ElfStubs.e_entry + begin_text;
    }
    
    WriteProgrammProlog(fasm, proga, stubs, machine_code);

    int size = lst.Size();
    DifferTree function;
    for (int i = 0; i < size; i++)
    {
        function = lst.ShowFront();
        fputc('\n', fasm);
        TreeTranslate(fasm, function, stubs, machine_code);
        lst.PopFront();
    }

    WriteStdFunctions(stubs, machine_code);

    uint64_t end_text = machine_code.Size();

    if (stubs.is_loading)
    {
        stubs.ElfStubs.text_stubs.p_size = end_text - begin_text;
    }
}

void VerifyDefFunc(node_t* function)
{
    if (function->dType() != DataType::DEFINE)
    {
        std::cout << *function << "\n";
        fprintf(stderr, "Error reading the function tree\n");
        exit(EXIT_FAILURE);
    }
}

void TreeTranslate(FILE* fasm, DifferTree& function, Stubs& stubs, ByteArray& machine_code)
{
    node_t* func = function.Root();
    VerifyDefFunc(func);

    func = func->GetRight();

    List<variable>* param = WriteFuncProlog(fasm, func, stubs, machine_code);
    int offset = param->Size() * sizeof(double);

    const char* funcname = func->Name();

    TranslateOpSequence(fasm, param, func->GetRight(), funcname, offset, stubs, machine_code);

    node_t null(NodeType::NUMBER, DataType::CONSTANT, 0.0);
    WriteFuncEpilog(fasm, param, &null, funcname, stubs, machine_code);
}

List<variable>* WriteFuncProlog(FILE* fasm, node_t* func, Stubs& stubs, ByteArray& machine_code)
{    
    const char* mark = func->Name();
    if (stubs.is_loading)
    {
        Label label_func(mark, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(label_func);
    }
    fprintf(fasm, "%s:\n", mark);
    
    List<variable>* variables = FillListVariables(func);

    fputs("\t\tpush\trbp\n", fasm);
    machine_code.Append(CMD::PUSH_RBP, 1);
    fputs("\t\tmov\t\trbp, rsp\n", fasm);
    machine_code.Append(CMD::MOV_RBP_RSP, 3);

    uint32_t size_stk_frame = SizeStackFrame(func, variables);
    fprintf(fasm, "\t\tsub\t\trsp, %d\n", size_stk_frame);
    AppendSubRspNum(size_stk_frame, machine_code);

    CopyParametrsToStack(fasm, variables, machine_code);
    return variables;
}

List<variable>* FillListVariables(node_t* node)
{
    VerifyFunc(node);
    List<variable>* variables = new List<variable>;

    if (node->GetLeft())
    {
        int offset = NumParam(node) * sizeof(double);
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
            offset -= sizeof(double);
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

void NumLocalVar(int& num_param, node_t* node)
{    
    if (node->dType() == DataType::INITIALIZATE)
    {
        num_param++;
        return;
    }
    if (node->GetLeft()) NumLocalVar(num_param, node->GetLeft());
    if (node->GetRight()) NumLocalVar(num_param, node->GetRight());

}

void VerifyFunc(node_t* node)
{
    if (node->dType() != DataType::FUNC)
    {
        std::cout << node << "\n";
        fprintf(stderr, "Error reading function parametrs");
        exit(EXIT_FAILURE);
    }
}

uint32_t SizeStackFrame(node_t* func, List<variable>* variables)
{
    int num_variables = 0 ;
    NumLocalVar(num_variables, func);
    num_variables += variables->Size();

    uint32_t size_stk_frame = 0;
    if (num_variables % 2 == 0)
    {
        size_stk_frame = num_variables * sizeof(double);
    }
    else
    {
        size_stk_frame = (num_variables + 1) * sizeof(double);
    }
    return size_stk_frame;
}

void CopyParametrsToStack(FILE* fasm, List<variable>* variables, ByteArray& machine_code)
{
    List<variable>* lst = new List<variable>(*variables);
    for (int i = 0, size = lst->Size(); i < size; i++)
    {
        variable var = lst->ShowFront();
        uint64_t offset = var.Offset();

        fprintf(fasm, "\t\tmov\t\trax, [rbp + %ld]\n", offset + sizeof(double));
        AppendMovRaxVarPtr(offset + sizeof(double), machine_code);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [rax]\n");
        machine_code.Append(CMD::MOVSD_XMM0_VAR, 4);
        fprintf(fasm, "\t\tmovsd\tqword [rbp - %ld], xmm0\n", offset);
        AppendMovsdVarXmm0(offset, machine_code);
        lst->PopFront();
    }
}

void WriteFuncEpilog(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    char* label_name = new char[strlen(funcname) + 6];
    sprintf(label_name, ".ret_%s", funcname);
    if (stubs.is_loading)
    {
        Label label_ret_func(label_name, LabelAddr(stubs, machine_code));
        stubs.labels.PushFront(label_ret_func);
    }
    fprintf(fasm, ".ret_%s:\n", funcname);
    
    for (int i = 0, size = param->Size(); i < size; i++)
    {
        variable var = param->ShowFront();
        if (var.IsLink())
        {
            uint64_t offset = var.Offset();
            fprintf(fasm, "\t\tmov\t\trax, [rbp + %ld]\n", offset + sizeof(double));
            AppendMovRaxVarPtr(offset + sizeof(double), machine_code);
            fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp - %ld]\n", offset);
            AppendMovsdXmm0Var(offset, machine_code);
            fprintf(fasm, "\t\tmovsd\tqword [rax], xmm0\n");
            machine_code.Append(CMD::MOVSD_XMM0_VAR, 4);
        }
        param->PopFront();
    }

    fputs("\t\tleave\n", fasm);
    machine_code.Append(CMD::LEAVE, 1);
    fputs("\t\tret\n", fasm);
    machine_code.Append(CMD::RET, 1);

    delete[] label_name;
}

void TranslateOpSequence(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    while (node && (node->dType() == DataType::END_OP))
    {
        TranslateOp(fasm, param, node->GetLeft(), funcname, offset, stubs, machine_code);
        node = node->GetRight();
    }
}

void TranslateOp(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    switch (node->dType())
    {
    case DataType::FUNC:
        TranslateCallFunc(fasm, param, node, funcname, stubs, machine_code);
        break;
    case DataType::PRINTF:
        TranslateCallPrintf(fasm, param, node, funcname, stubs, machine_code);
        break;
    case DataType::SCANF:
        TranslateCallScanf(fasm, param, node, funcname, machine_code);
        break;
    case DataType::SQRT:
        TransateCallSqtr(fasm, param, node, funcname, stubs, machine_code);
        break;
    case DataType::INITIALIZATE:
        TranslateInit(fasm, param, node, funcname, offset, stubs, machine_code);
        break;
    case DataType::MOV:
        TranslateMov(fasm, param, node, funcname, stubs, machine_code);
        break;
    case DataType::IF:
        TranslateIf(fasm, param, node, funcname, offset, stubs, machine_code);
        break;
    case DataType::WHILE:
        TranslateWhile(fasm, param, node, funcname, offset, stubs, machine_code);
        break;
    case DataType::RET:
        TranslateRet(fasm, param, node, funcname, stubs, machine_code);
        break;
    default:
        break;
    }
}

void TranslateCallFunc(FILE* fasm, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const char* call_func = node->Name();
    node = node->GetRight();

    int number_op = 0;
    if (node)
    {
        uint64_t offset = 0;
        while (node->dType() == DataType::COMMA)
        {                    
            offset = OffsetVariable(lst, node->GetRight());
            fprintf(fasm, "\t\tlea\t\trax, [rbp - %ld]\n", offset);
            AppendLeaRaxVar(offset, machine_code);
            fprintf(fasm, "\t\tpush\trax\n");
            machine_code.Append(CMD::PUSH_RAX, 1);
            node = node->GetLeft();
            number_op++;
        }
        offset = OffsetVariable(lst, node);
        fprintf(fasm, "\t\tlea\t\trax, [rbp - %ld]\n", offset);
        AppendLeaRaxVar(offset, machine_code);
        fprintf(fasm, "\t\tpush\trax\n");
        machine_code.Append(CMD::PUSH_RAX, 1);
        number_op++;
    }
    
    fprintf(fasm, "\t\tcall\t%s\n", call_func);
    AppendCallFunc(call_func, stubs, machine_code);
    fprintf(fasm, "\t\tadd\t\trsp, %ld\n", number_op * sizeof(double));
    AppendAddRspNum(number_op * sizeof(double), machine_code);
}

void TransateCallSqtr(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    node = node->GetRight();
    TranslateExp(fasm, param, node, funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tsqrtsd\txmm0, [buffer]\n");
    machine_code.Append(CMD::SQRTSD_XMM0_M64, 5);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
    machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tpush\tqword [buffer]\n");
    machine_code.Append(CMD::PUSH_M64, 3);
    machine_code.Append(BUF);
}

void TranslateInit(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, int initial_offset, Stubs& stubs, ByteArray& machine_code)
{    
    node = node->GetRight();
    static const char* last_call_func = funcname;
    static int offset = initial_offset + sizeof(double);
    static int offset_save = offset;
    if (stubs.rewind_init)
    {
        offset = offset_save;
        stubs.rewind_init = false;
    }

    if (last_call_func != funcname)
    {
        offset = initial_offset + sizeof(double);
    }
    if (node->dType() == DataType::VARIABLE)
    {
        variable var(node, false, offset);
        param->PushFront(var);
    }
    else if (node->dType() == DataType::MOV)
    {
        variable var(node->GetLeft(), false, offset);
        param->PushFront(var);        
        TranslateMov(fasm, param, node, funcname, stubs, machine_code);
    }
    offset += sizeof(double);
    last_call_func = funcname;
}

void TranslateMov(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    TranslateExp(fasm, param, node->GetRight(), funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
    machine_code.Append(BUF);
    uint64_t offset = OffsetVariable(param, node->GetLeft());
    fprintf(fasm, "\t\tmovsd\tqword [rbp - %ld], xmm0\n", offset);
    AppendMovsdVarXmm0(offset, machine_code);
}

void TranslateCallScanf(FILE* fasm, List<variable>* lst, node_t* node, const char* funcname, ByteArray& machine_code)
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

void TranslateRet(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    node = node->GetRight();
    TranslateExp(fasm, param, node, funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
    machine_code.Append(BUF);

    char* labelname = new char[strlen(funcname) + 6];
    sprintf(labelname, ".ret_%s", funcname);
    fprintf(fasm, "\t\tjmp\t\t%s\n", labelname);
    AppendJmpLabel(labelname, stubs, machine_code);
    delete[] labelname;
}

void TranslateCallPrintf(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    node = node->GetRight();

    List<node_t*> StackNodePtr; 
    while (node && (node->dType() == DataType::COMMA))
    {
        StackNodePtr.PushBack(node);
        node = node->GetLeft();
    }

    AppendAddRspNum(8, machine_code);

    uint64_t offset = OffsetVariable(param, node);
    fprintf(fasm, "\t\tmov\t\trdi, print_double\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp - %ld]\n", offset);
    fprintf(fasm, "\t\tmov\t\teax, 1\n");
    fprintf(fasm, "\t\tcall\tprintf\n");

    AppendSubRspNum(8, machine_code);

    AppendMovsdXmm0Var(offset, machine_code);
    AppendMovRdiStr(stubs, machine_code);
    AppendCallFunc("dtoa", stubs, machine_code);
    machine_code.Append(CMD::MOV_RDX_RAX, 3);
    machine_code.Append(CMD::MOV_RAX_1, 7);
    machine_code.Append(CMD::MOV_RDI_1, 7);
    AppendMovRsiStr(stubs, machine_code);
    machine_code.Append(CMD::SYSCALL, 2);
    


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

void TranslateWhile(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    static int num_while = -1;
    if (stubs.rewind_while)
    {
        num_while = -1;
        stubs.rewind_while = false;
    }
    num_while++;
    int save_num = num_while;
    node_t* condition = node->GetLeft();
    
    char* while_test = new char[20];
    snprintf(while_test, 20, ".while%dtest", save_num);
    char* while_loop = new char[20];
    snprintf(while_loop, 20, ".while%dloop", save_num);
    
    fprintf(fasm, "\t\tjmp\t\t%s\n", while_test);
    AppendJmpLabel(while_test, stubs, machine_code);

    fprintf(fasm, ".while%dloop:\n", save_num);
    if (stubs.is_loading)
    {
        Label loop(while_loop, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(loop);
    }

    TranslateOpSequence(fasm, param, node->GetRight(), funcname, offset, stubs, machine_code);

    fprintf(fasm, ".while%dtest:\n", save_num);
    if (stubs.is_loading)
    {
        Label test(while_test, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(test);
    }

    TranslateExp(fasm, param, condition->GetLeft(), funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
    machine_code.Append(BUF);
    TranslateExp(fasm, param, condition->GetRight(), funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm1, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tcomisd\txmm0, xmm1\n");
    machine_code.Append(CMD::COMISD_XMM0_XMM1, 4);
    fprintf(fasm, "\t\t%s\t\t%s\n", Jxx(condition), while_loop);
    AppendJxxLabel(while_loop, condition, stubs, machine_code);

    delete[] while_test;
    delete[] while_loop;
}

void TranslateIf(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    static int num_if = -1;
    if (stubs.rewind_if)
    {
        num_if = -1;
        stubs.rewind_if = false;
    }
    num_if++;
    int save_num = num_if;

    node_t* condition = node->GetLeft();
    TranslateExp(fasm, param, condition->GetLeft(), funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
    machine_code.Append(BUF);
    TranslateExp(fasm, param, condition->GetRight(), funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm1, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tcomisd\txmm0, xmm1\n");
    machine_code.Append(CMD::COMISD_XMM0_XMM1, 4);


    char* if_else = new char[20];
    snprintf(if_else, 20, ".if%delse", save_num);
    char* if_end = new char[20];
    snprintf(if_end, 20, ".if%dend", save_num);

    if (node->GetRight()->dType() == DataType::ELSE)
    {
        fprintf(fasm, "\t\t%s\t\t%s\n", Jnx(condition), if_else);
        AppendJnxLabel(if_else, condition, stubs, machine_code);

        node_t* op_else = node->GetRight();
        TranslateOpSequence(fasm, param, op_else->GetLeft(), funcname, offset, stubs, machine_code);

        fprintf(fasm, "\t\tjmp\t\t%s\n", if_end);
        AppendJmpLabel(if_end, stubs, machine_code);

        fprintf(fasm, "%s:\n", if_else);
        if (stubs.is_loading)
        {
            Label if_else_lbl(if_else, LabelAddr(stubs, machine_code));
            stubs.labels.PushBack(if_else_lbl);
        }

        if (op_else->GetRight()->dType() != DataType::IF)
        {
            TranslateOpSequence(fasm, param, op_else->GetRight(), funcname, offset, stubs, machine_code);
        }
        else
        {
            TranslateIf(fasm, param, op_else->GetRight(), funcname, offset, stubs, machine_code);
        }

        fprintf(fasm, "%s:\n", if_end);
        if (stubs.is_loading)
        {
            Label if_end_lbl(if_end, LabelAddr(stubs, machine_code));
            stubs.labels.PushBack(if_end_lbl);
        }        
    }
    else
    {
        fprintf(fasm, "\t\t%s\t\t%s\n", Jnx(condition), if_end);
        AppendJnxLabel(if_end, condition, stubs, machine_code);

        TranslateOpSequence(fasm, param, node->GetRight(), funcname, offset, stubs, machine_code);
        
        fprintf(fasm, "%s:\n", if_end);
        if (stubs.is_loading)
        {
            Label if_end_lbl(if_end, LabelAddr(stubs, machine_code));
            stubs.labels.PushBack(if_end_lbl);
        }  
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

void TranslateExp(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{    
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    switch (node->dType())
    {
    case DataType::VARIABLE:
    {
        uint64_t offset = OffsetVariable(param, node);
        fprintf(fasm, "\t\tpush\tqword [rbp - %ld]\n", offset);
        AppendPushVar(offset, machine_code);
        break;
    }
    case DataType::ADD:
        TranslateExp(fasm, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, param, node->GetRight(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm1, [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\taddsd\txmm0, xmm1\n");
        machine_code.Append(CMD::ADDSD_XMM0_XMM1, 4);
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::SUB:
        if (node->GetLeft())
        {
            TranslateExp(fasm, param, node->GetLeft(), funcname, stubs, machine_code);
        }
        else
        {
            fprintf(fasm, "\t\tfldz\n");
            machine_code.Append(CMD::FLDZ, 2);
            fprintf(fasm, "\t\tfstp\tqword [buffer]\n");
            machine_code.Append(CMD::FSTP_M64, 3);
            machine_code.Append(BUF);
            fprintf(fasm, "\t\tpush\tqword [buffer]\n");
            machine_code.Append(CMD::PUSH_M64, 1);
            machine_code.Append(BUF);
        }
        TranslateExp(fasm, param, node->GetRight(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm1, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tsubsd\txmm0, xmm1\n");
        machine_code.Append(CMD::SUBSD_XMM0_XMM1, 4);
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::MUL:
        TranslateExp(fasm, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, param, node->GetRight(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm1, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmulsd\txmm0, xmm1\n");
        machine_code.Append(CMD::MULSD_XMM0_XMM1, 4);
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::DIV:
        TranslateExp(fasm, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, param, node->GetRight(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm1, [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tdivsd\txmm0, xmm1\n");
        machine_code.Append(CMD::DIVSD_XMM0_XMM1, 4);
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::CONSTANT:
        static int num_const = 0;
        if (stubs.rewind_const_expr)
        {
            num_const = 0;
            stubs.rewind_const_expr = false;
        }   
        fprintf(fasm, "\t\tpush\tqword [const_%d]\n", num_const);
        AppendPushConst(num_const++, stubs, machine_code);
        break;
    case DataType::FUNC:
        TranslateCallFunc(fasm, param, node, funcname, stubs, machine_code);
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::SQRT:
        TransateCallSqtr(fasm, param, node, funcname, stubs, machine_code);
        break;   
    default:
        break;
    }
}

void WriteProgrammProlog(FILE* fasm, List<DifferTree>& tree, Stubs& stubs, ByteArray& machine_code)
{   
    const char* programm_name = MainFuncName(tree);
    assert(programm_name);
    
    fprintf(fasm, "main:\n");
    fprintf(fasm, "\t\tfinit\n");
    fprintf(fasm, "\t\tcall\t%s\n", programm_name);

    fprintf(fasm, "\t\tmov\t\trax, 60\n");
    fprintf(fasm, "\t\txor\t\trdi, rdi\n");
    fprintf(fasm, "\t\tsyscall\n");

    machine_code.Append(CMD::FINIT, 3);
    AppendCallFunc(programm_name, stubs, machine_code);
    
    machine_code.Append(CMD::MOV_RAX_60, 7);
    machine_code.Append(CMD::XOR_RDI_RDI, 3);
    machine_code.Append(CMD::SYSCALL, 2);
}

void WriteStdFunctions(Stubs& stubs, ByteArray& machine_code)
{
    LOX
    
    //WritePow(stubs, machine_code);
    //  WriteLog10(stubs, machine_code);
    //WriteIsNan(stubs, machine_code);
    //WriteIsInf(stubs, machine_code);
    //WriteStrlen(stubs, machine_code);
    //WriteFloor(stubs, machine_code);
    WriteDtoa(stubs, machine_code);

    LOX
}

void WritePow(Stubs& stubs, ByteArray& machine_code)
{   
    addr_t buf_ptr = stubs.ElfStubs.data_stubs.p_vaddp;
    uint8_t* buf = (uint8_t*)memreverse(&buf_ptr, 4); 
    
                        //////  00    01    02    03      04      05      06      07      08      09    0A    0B    0C      0D      0E      0F
    uint8_t pow[0x48] = /*00*/ {0xF2, 0x0F, 0x11, 0x0C,   0x25,   buf[0], buf[1], buf[2], buf[3], 0xDD, 0x04, 0x25, buf[0], buf[1], buf[2], buf[3],
                        /*10*/  0xF2, 0x0F, 0x11, 0x04,   0x25,   buf[0], buf[1], buf[2], buf[3], 0xDD, 0x04, 0x25, buf[0], buf[1], buf[2], buf[3],
                        /*20*/  0xD9, 0xF1, 0xD9, 0xC0,   0xD9,   0xFC,   0xD8,   0xE9,   0xD9,   0xF0, 0xD9, 0xE8, 0xDE,   0xC1,   0xD9,   0xFD,
                        /*30*/  0xDD, 0x1C, 0x25, buf[0], buf[1], buf[2], buf[3], 0xF2,   0x0F,   0x10, 0x04, 0x25, buf[0], buf[1], buf[2], buf[3],
                        /*40*/  0xDD, 0x1C, 0x25, buf[0], buf[1], buf[2], buf[3], 0xC3};

    stubs.pow_addres = machine_code.Vaddr();
    machine_code.Append(pow, 0x48);
}

void WriteLog10(Stubs& stubs, ByteArray& machine_code)
{   
    addr_t buf_ptr = stubs.ElfStubs.data_stubs.p_vaddp;
    uint8_t* buf = (uint8_t*)memreverse(&buf_ptr, 4); 
    
                        //////  00      01      02    03    04      05      06      07      08      09    0A    0B      0C      0D      0E      0F
    uint8_t log[0x29] = /*00*/ {0xF2,   0x0F,   0x11, 0x04, 0x25,   buf[0], buf[1], buf[2], buf[3], 0xD9, 0xE8, 0xDD,   0x04,   0x25,   buf[0], buf[1],
                        /*10*/  buf[2], buf[3], 0xD9, 0xF1, 0xD9,   0xE9,   0xDE,   0xF9,   0xDD,   0x1C, 0x25, buf[0], buf[1], buf[2], buf[3], 0xF2,
                        /*20*/  0x0F,   0x10,   0x04, 0x25, buf[0], buf[1], buf[2], buf[3], 0xC3};

    stubs.log10_adderes = machine_code.Vaddr();
    machine_code.Append(log, 0x29);
}


void WriteIsNan(Stubs& stubs, ByteArray& machine_code)
{    
                          //////  00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    uint8_t isnan[0x24] = /*00*/ {0x55, 0x48, 0x89, 0xE5, 0xF2, 0x0F, 0x11, 0x45, 0xF8, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xF2, 0x0F,
                          /*10*/  0x10, 0x45, 0xF8, 0x66, 0x0F, 0x2E, 0x45, 0xF8, 0x7A, 0x05, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x0F,
                          /*20*/  0xB6, 0xC0, 0x5D, 0xC3};

    stubs.isnan_addres = machine_code.Vaddr();
    machine_code.Append(isnan, 0x24);   
}

void WriteIsInf(Stubs& stubs, ByteArray& machine_code)
{    
                          //////  00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    uint8_t isinf[0x49] = /*00*/ {0x55, 0x48, 0x89, 0xE5, 0x48, 0x83, 0xEC, 0x08, 0xF2, 0x0F, 0x11, 0x45, 0xF8, 0x48, 0x8B, 0x45,
                          /*10*/  0xF8, 0x66, 0x48, 0x0F, 0x6E, 0xC0, 0xE8, 0xC1, 0xFF, 0xFF, 0xFF, 0x85, 0xC0, 0x75, 0x23, 0xF2, 
                          /*20*/  0x0F, 0x10, 0x45, 0xF8, 0xF2, 0x0F, 0x5C, 0xC0, 0x66, 0x48, 0x0F, 0x7E, 0xC0, 0x66, 0x48, 0x0F, 
                          /*30*/  0x6E, 0xC0, 0xE8, 0xA5, 0xFF, 0xFF, 0xFF, 0x85, 0xC0, 0x74, 0x07, 0xB8, 0x01, 0x00, 0x00, 0x00, 
                          /*40*/  0xEB, 0x05, 0xB8, 0x00, 0x00, 0x00, 0x00, 0xC9, 0xC3};

    stubs.isinf_addres = machine_code.Vaddr();
    machine_code.Append(isinf, 0x49);   
}

void WriteStrlen(Stubs& stubs, ByteArray& machine_code)
{    
                           //////  00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    uint8_t strlen[0x2A] = /*00*/ {0x55, 0x48, 0x89, 0xE5, 0x48, 0x89, 0x7D, 0xE8, 0xC7, 0x45, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xEB,
                           /*10*/  0x09, 0x83, 0x45, 0xFC, 0x01, 0x48, 0x83, 0x45, 0xE8, 0x01, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xB6,
                           /*20*/  0x00, 0x84, 0xC0, 0x75, 0xEC, 0x8B, 0x45, 0xFC, 0x5D, 0xC3};

    stubs.strlen_addres = machine_code.Vaddr();
    machine_code.Append(strlen, 0x2A);   
}

void WriteFloor(Stubs& stubs, ByteArray& machine_code)
{    
                           //////  00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    uint8_t floor[0x56] =  /*00*/ {0x55, 0x48, 0x89, 0xE5, 0xF2, 0x0F, 0x11, 0x45, 0xF8, 0xF2, 0x0F, 0x10, 0x45, 0xF8, 0x66, 0x0F,
                           /*10*/  0xEF, 0xC9, 0x66, 0x0F, 0x2F, 0xC1, 0x72, 0x13, 0xF2, 0x0F, 0x10, 0x45, 0xF8, 0xF2, 0x0F, 0x2C,
                           /*20*/  0xC0, 0x66, 0x0F, 0xEF, 0xC0, 0xF2, 0x0F, 0x2A, 0xC0, 0xEB, 0x1F, 0xF2, 0x0F, 0x10, 0x45, 0xF8,
                           /*30*/  0xD9, 0xE8, 0xDD, 0x5D, 0xF0, 0xF2, 0x0F, 0x10, 0x4D, 0xF0, 0xF2, 0x0F, 0x5C, 0xC1, 0xF2, 0x0F,
                           /*40*/  0x2C, 0xC0, 0x66, 0x0F, 0xEF, 0xC0, 0xF2, 0x0F, 0x2A, 0xC0, 0x66, 0x48, 0x0F, 0x7E, 0xC0, 0x66,
                           /*50*/  0x48, 0x0F, 0x6E, 0xC0, 0x5D, 0xC3};

    stubs.floor_addres = machine_code.Vaddr();
    machine_code.Append(strlen, 0x56);   
}

void WriteDtoa(Stubs& stubs, ByteArray& machine_code)
{    
    // FILE* dtoa_file = fopen("dtoa", "rb");
    // uint8_t* dtoa = new uint8_t[0x4BA] {0};
    // fread(dtoa, 1, 0x4BA, dtoa_file);

    // stubs.dtoa_addres = machine_code.Vaddr();
    // stubs.labels.PushBack(Label("dtoa", stubs.dtoa_addres));
    // machine_code.Append(dtoa, 0x4BA);   

    // fclose(dtoa_file);
    // delete[] dtoa;

    LOX
    
    FILE* func = fopen("all", "rb");
    uint8_t* all = new uint8_t[0x5f6] {0};
    fread(all, 1, 0x5f6, func);

    stubs.dtoa_addres = machine_code.Vaddr() + 0x13c;
    stubs.labels.PushBack(Label("dtoa", stubs.dtoa_addres));
    stubs.labels.PushBack(Label("pow", machine_code.Vaddr() ));
    machine_code.AppendBin(all, 0x5f6);   

    fclose(func);
    delete[] all;

}