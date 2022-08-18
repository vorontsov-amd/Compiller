#include "Compiliter.h"

#undef LOX
#define LOX 


void TranslateToAsm(List<DifferTree>& proga, const char* out_name)
{
    const char* programm_name = ProgrammName(proga);

    FILE* fasm = fopen(programm_name, "w");

    ByteArray machine_code;
    Stubs& stubs = machine_code.stubs;

    machine_code.AppendElfHeader();
    WritePreamble(fasm, proga, machine_code);
    TranslateProcessing(fasm, proga, stubs, machine_code);


    stubs.is_loading = false;
    stubs.reset_const = true;
    stubs.reset_if = true;
    stubs.reset_while = true;
    stubs.reset_init = true;
    stubs.reset_const_expr = true;
    stubs.reset_const_str = true;
    stubs.reset_const_str_printf = true;
    rewind(fasm);
    machine_code.Rewind();

    machine_code.AppendElfHeader();
    WritePreamble(fasm, proga, machine_code);
    TranslateProcessing(fasm, proga, stubs, machine_code);
    FILE* out = fopen(out_name, "wb");    
    assert(out);
    LOX
    fwrite(machine_code.ByteCode(), 1, machine_code.Size(), out);
    LOX
    fclose(fasm);
    fclose(out);
    LOX
}



const char* ProgrammName(List<DifferTree>& proga)
{
    const char* name = FrontFuncName(proga);
    char* fullname = new char[strlen(name) + 5];
    strcpy(fullname, name);
    strcat(fullname, ".asm");
    return fullname;
}

const char* FrontFuncName(List<DifferTree>& proga)
{
    DifferTree main_function = proga.ShowFront();
    node_t function = main_function.Root()->GetRight();
    return function.Name();
}

void WritePreamble(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code)
{
    fputs("global _start\n", fasm);
    fputs("extern dtoa, atod, pow\n\n", fasm);
    PreambleData(fasm, proga, machine_code);
    PreambleRodata(fasm, proga, machine_code);
    fputs("section .text\n", fasm);
}

void PreambleRodata(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code)
{
    fputs("section .rodata\n", fasm);

    uint64_t rodata_begin = machine_code.Size();
    if (machine_code.stubsNotLoaded())
    {
        machine_code.rodataStubs().p_offset = rodata_begin;
        machine_code.rodataStubs().p_vaddp  = machine_code.e_point() + rodata_begin;
    }

    WriteConstant(fasm, DataType::CONSTANT, proga, machine_code);
    WriteConstant(fasm, DataType::CONST_STR, proga, machine_code);


    uint16_t rodata_end = machine_code.Size();
    if (machine_code.stubsNotLoaded())
    {
        machine_code.rodataStubs().p_size = rodata_end - rodata_begin;
    }
}

void PreambleData(FILE* fasm, List<DifferTree>& proga, ByteArray& machine_code)
{
    fputs("section .data\n", fasm);

    if (machine_code.stubsNotLoaded())
    {
        machine_code.dataStubs().p_offset = machine_code.Size();
        machine_code.dataStubs().p_vaddp = machine_code.e_point() + machine_code.Size();
        machine_code.dataStubs().p_size = sizeof(double);
    }

    fputs("buffer: dq 0.0\n", fasm);
    machine_code.Append(0.0);

    fputs("str: times 32 db 0\n", fasm);
    machine_code.Append(0U);
    machine_code.Append(0U);
    machine_code.Append(0U);
    machine_code.Append(0U);
}

void WriteConstant(FILE* fasm, DataType::dataType mode, List<DifferTree> proga, ByteArray& machine_code)
{       
    int size = proga.Size();
    for (int i = 0; i < size; i++)
    {
        DifferTree tree = proga.ShowFront();
        SearchConst(fasm, mode, tree.Root(), machine_code);
        proga.PopFront();
    }
}

void SearchConst(FILE* fasm, DataType::dataType mode, node_t* node, ByteArray& machine_code)
{
    if (node->GetLeft()) SearchConst(fasm, mode, node->GetLeft(), machine_code);
    if (node->dType() == mode)
    {
        auto AppendData = AppendConst;
        switch (mode)
        {
        case DataType::CONST_STR:
            AppendData = AppendStr;
            break;
        case DataType::CONSTANT:
            AppendData = AppendConst;
            break;
        default:
            fprintf(stderr, "Incorrect Searching mode");
            exit(EXIT_FAILURE);
            break;
        }
        AppendData(fasm, node, machine_code);
    }
    if (node->GetRight()) SearchConst(fasm, mode, node->GetRight(), machine_code);
}

void AppendConst(FILE* fasm, node_t* node, ByteArray& machine_code)
{    
    static int num_const = 0;
    if (machine_code.resetConstDeclCounter())
    {
        num_const = 0;
    }

    double number = node->Num();
    std::string lbl_number = "const_" + std::to_string(num_const);

    fprintf(fasm, "%s: dq %lf\n", lbl_number.c_str(), number);

    machine_code.AddLabel(lbl_number);
    machine_code.Append(number);

    num_const++;
}

void AppendStr(FILE* fasm, node_t* node, ByteArray& machine_code)
{
    static int num_const = 0;
    if (machine_code.resetStrDeclCounter())
    {
        num_const = 0;
    }

    const char* str = node->Name();
    std::string str_mark = "str_" + std::to_string(num_const);

    fprintf(fasm, "%s: db '", str_mark.c_str());
    fputs(str, fasm);
    fputs("'\n", fasm);

    uint32_t str_len = strlen(str);

    machine_code.AddLabel(str_mark);
    machine_code.Append(str_len);
    machine_code.Append(str, str_len);

    num_const++;
}

void TranslateProcessing(FILE* fasm, List<DifferTree> proga, Stubs& stubs, ByteArray& machine_code)
{
    List<node_t>* functions = CreateLstFuncNode(proga);
    
    uint64_t begin_text = machine_code.Size();
    if (machine_code.stubsNotLoaded())
    {
        machine_code.textStubs().p_offset = begin_text;
        machine_code.textStubs().p_vaddp = machine_code.e_point() + begin_text;
    }
    
    WriteProgrammProlog(fasm, proga, machine_code);

    int size = proga.Size();
    for (int i = 0; i < size; i++)
    {
        DifferTree function = proga.ShowFront();
        fputc('\n', fasm);
        TreeTranslate(fasm, function, functions, stubs, machine_code);
        proga.PopFront();
    }

    machine_code.AppendStdFunctions();

    uint64_t end_text = machine_code.Size();
    if (machine_code.stubsNotLoaded())
    {
        machine_code.textStubs().p_size = end_text - begin_text;
    }

    delete functions;
}

List<node_t>* CreateLstFuncNode(List<DifferTree> proga)
{
    List<node_t>* lst = new List<node_t>;

    for (int i = 0, size = proga.Size(); i < size; i++)
    {        
        lst->PushBack(*proga.ShowFront().Root()->GetRight());
        proga.PopFront();
    }

    return lst;
}

void WriteProgrammProlog(FILE* fasm, List<DifferTree>& tree, ByteArray& machine_code)
{   
    const char* programm_name = FrontFuncName(tree);
    
    fprintf(fasm, "_start:\n");
    fprintf(fasm, "\t\tfinit\n");
    fprintf(fasm, "\t\tcall\t%s\n", programm_name);

    fprintf(fasm, "\t\tmov\t\trax, 60\n");
    fprintf(fasm, "\t\txor\t\trdi, rdi\n");
    fprintf(fasm, "\t\tsyscall\n");

    machine_code.AppendCmd(CMD::FINIT, 3);
    machine_code.AppendCallFunc(programm_name);
    machine_code.AppendCmd(CMD::MOV_RAX_60, 7);
    machine_code.AppendCmd(CMD::XOR_RDI_RDI, 3);
    machine_code.AppendCmd(CMD::SYSCALL, 2);
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

void TreeTranslate(FILE* fasm, DifferTree& function, List<node_t>* functions, Stubs& stubs, ByteArray& machine_code)
{
    node_t* func = function.Root();
    VerifyDefFunc(func);

    func = func->GetRight();

    List<variable>* param = WriteFuncProlog(fasm, func, stubs, machine_code);
    int offset = param->Size() * sizeof(double);

    const char* funcname = func->Name();

    TranslateOpSequence(fasm, functions, param, func->GetRight(), funcname, offset, stubs, machine_code);

    node_t null(NodeType::NUMBER, DataType::CONSTANT, 0.0);
    WriteFuncEpilog(fasm, param, &null, funcname, stubs, machine_code);

    delete param;
}

List<variable>* WriteFuncProlog(FILE* fasm, node_t* func, Stubs& stubs, ByteArray& machine_code)
{    
    const char* mark = func->Name();
    
    fprintf(fasm, "%s:\n", mark);
    if (machine_code.stubsNotLoaded())
    {
        machine_code.AddLabel(mark);
    }
    
    List<variable>* variables = FillListVariables(func);

    fputs("\t\tpush\trbp\n", fasm);
    fputs("\t\tmov\t\trbp, rsp\n", fasm);
    machine_code.AppendCmd(CMD::PUSH_RBP, 1);
    machine_code.AppendCmd(CMD::MOV_RBP_RSP, 3);

    uint32_t size_stk_frame = SizeStackFrame(func, variables);

    fprintf(fasm, "\t\tsub\t\trsp, %d\n", size_stk_frame);
    machine_code.AppendCmd(CMD::SUB_RSP_NUM_L, CMD::SUB_RSP_NUM_B, 4, size_stk_frame);

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
    return num_variables * sizeof(double);
}

void CopyParametrsToStack(FILE* fasm, List<variable>* variables, ByteArray& machine_code)
{
    List<variable>* lst = new List<variable>(*variables);
    for (int i = 0, size = lst->Size(); i < size; i++)
    {
        variable var = lst->ShowFront();
        uint32_t offset = var.Offset();
        if (var.IsLink())
        {
            fprintf(fasm, "\t\tmov\t\trax, [rbp + %ld]\n", offset + sizeof(double));
            fprintf(fasm, "\t\tmovsd\txmm0, qword [rax]\n");
            fprintf(fasm, "\t\tmovsd\tqword [rbp - %d], xmm0\n", offset);
            machine_code.AppendCmd(CMD::MOV_RAX_VAR_L, CMD::MOV_RAX_VAR_B, 4, offset + sizeof(double));
            machine_code.AppendCmd(CMD::MOVSD_XMM0_VAR, 4);
            machine_code.AppendCmd(CMD::MOVSD_VAR_XMM0_L, CMD::MOVSD_VAR_XMM0_B, 5, -offset);
        }
        else
        {
            fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp + %ld]\n", offset + sizeof(double));
            fprintf(fasm, "\t\tmovsd\tqword [rbp - %d], xmm0\n", offset);
            machine_code.AppendCmd(CMD::MOVSD_XMM0_VAR_L, CMD::MOVSD_XMM0_VAR_B, 5, offset + sizeof(double));
            machine_code.AppendCmd(CMD::MOVSD_VAR_XMM0_L, CMD::MOVSD_VAR_XMM0_B, 5, -offset);
        }
        lst->PopFront();
    }
}

void WriteFuncEpilog(FILE* fasm, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    std::string ret_lbl = std::string(".ret_") + funcname;

    fprintf(fasm, ".ret_%s:\n", funcname);
    if (machine_code.stubsNotLoaded())
    {
        machine_code.AddLabel(ret_lbl);
    }
    
    for (int i = 0, size = param->Size(); i < size; i++)
    {
        variable var = param->ShowFront();
        if (var.IsLink())
        {
            uint32_t offset = var.Offset();
            fprintf(fasm, "\t\tmov\t\trax, [rbp + %ld]\n", offset + sizeof(double));
            fprintf(fasm, "\t\tmovsd\txmm0, qword [rbp - %d]\n", offset);
            fprintf(fasm, "\t\tmovsd\tqword [rax], xmm0\n");
            machine_code.AppendCmd(CMD::MOV_RAX_VAR_L, CMD::MOV_RAX_VAR_B, 4, offset + sizeof(double));
            machine_code.AppendCmd(CMD::MOVSD_XMM0_VAR_L, CMD::MOVSD_XMM0_VAR_B, 5, -offset);
            machine_code.AppendCmd(CMD::MOVSD_VAR_XMM0, 4);
        }
        param->PopFront();
    }

    fputs("\t\tleave\n", fasm);
    fputs("\t\tret\n", fasm);
    machine_code.AppendCmd(CMD::LEAVE, 1);
    machine_code.AppendCmd(CMD::RET, 1);
}

void TranslateOpSequence(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    while (node && (node->dType() == DataType::END_OP))
    {
        TranslateOp(fasm, functions, param, node->GetLeft(), funcname, offset, stubs, machine_code);
        node = node->GetRight();
    }
}

void TranslateOp(FILE* fasm, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    static int num_const_str = 0;
    if (machine_code.resetStrPrintCounter())
    {
        num_const_str = 0;
    } 
    
    switch (node->dType())
    {
    case DataType::FUNC:
        TranslateCallFunc(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::PRINTF:
        TranslateCallPrintf(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::SCANF:
        TranslateCallScanf(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::SQRT:
        TransateCallSqtr(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        //CLEAR STACK
        break;
    //CASE SIN COS LOG
    case DataType::INITIALIZATE:
        TranslateInit(fasm, num_const_str, functions, param, node, funcname, offset, stubs, machine_code);
        break;
    case DataType::MOV:
        TranslateMov(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::IF:
        TranslateIf(fasm, num_const_str, functions, param, node, funcname, offset, stubs, machine_code);
        break;
    case DataType::WHILE:
        TranslateWhile(fasm, num_const_str, functions, param, node, funcname, offset, stubs, machine_code);
        break;
    case DataType::RET:
        TranslateRet(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    default:
        break;
    }
}

void TranslateCallFunc(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const char* call_func_name = node->Name();
    node = node->GetRight();

    int number_op = 0;
    if (node)
    {
        try
        {
            node_t call_func_param = SearchCallFunc(call_func_name, functions).GetLeft();
            while (node->dType() == DataType::COMMA && call_func_param.dType() == DataType::COMMA)
            {                    
                TransferParamToFunc(fasm, num_const_str, node->GetRight(), call_func_param.GetRight(), functions, lst, funcname, stubs, machine_code);
                node = node->GetLeft();
                call_func_param = call_func_param.GetLeft();
                number_op++;
            }
            TransferParamToFunc(fasm, num_const_str, node, call_func_param, functions, lst, funcname, stubs, machine_code);
            number_op++;
        }
        catch (bad_call& exc)
        {
            std::cout << exc.what();
            exit(EXIT_FAILURE);
        }
    }
    
    fprintf(fasm, "\t\tcall\t%s\n", call_func_name);
    machine_code.AppendCallFunc(call_func_name);
    fprintf(fasm, "\t\tadd\t\trsp, %ld\n", number_op * sizeof(double));
    AppendAddRspNum(number_op * sizeof(double), machine_code);
}

node_t SearchCallFunc(const char* func_name, List<node_t>* functions)
{
    List<node_t>* functions_copy = new List<node_t> (*functions);
    for (int i = 0, size = functions->Size(); i < size; i++)
    {
        node_t target = functions_copy->ShowFront();
        if (strcmp(target.Name(), func_name) == 0)
        {
            delete functions_copy;
            return target;
        }
        functions_copy->PopFront();
    }
    delete functions_copy;
    throw bad_call(func_name);
}


void TransferParamToFunc(FILE* fasm, int& num_const_str, node_t* param, node_t param_call_func, List<node_t>* functions, List<variable>* lst, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    if (param_call_func.dType() == DataType::NEW_VAR)
    {
        TranslateExp(fasm, num_const_str, functions, lst, param, funcname, stubs, machine_code);
    }
    else if (param->dType() == DataType::VARIABLE)
    {
        uint64_t offset = OffsetVariable(lst, param);
        fprintf(fasm, "\t\tlea\t\trax, [rbp - %ld]\n", offset);
        AppendLeaRaxVar(offset, machine_code);
        fprintf(fasm, "\t\tpush\trax\n");
        machine_code.Append(CMD::PUSH_RAX, 1);    
    }
    else
    {
        fprintf(stderr, "Error: Attempt to assign an rvalue to an lvalue\n");
        exit(EXIT_FAILURE);
    }
}


void TransateCallSqtr(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    node = node->GetRight();
    TranslateExp(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
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

void TranslateInit(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int initial_offset, Stubs& stubs, ByteArray& machine_code)
{    
    node = node->GetRight();
    static const char* last_call_func = funcname;
    static int offset = initial_offset + sizeof(double);
    static int offset_save = offset;
    if (stubs.reset_init)
    {
        offset = offset_save;
        stubs.reset_init = false;
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
        TranslateMov(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
    }
    offset += sizeof(double);
    last_call_func = funcname;
}

void TranslateMov(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
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

void TranslateCallScanf(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    node_t* param_seq = node->GetRight();
    if (param_seq && NoStringArgument(param_seq))
    {
        TranslateScanfMoreParametrs(fasm, lst, param_seq, stubs, machine_code);
    }
    else
    {
        LOX
        TranslateScanfReturn(fasm, num_const_str, functions, lst, node, funcname, stubs, machine_code);
        LOX
    }
}

bool NoStringArgument(node_t* node)
{
    if (node->dType() == DataType::COMMA)
    {
        return NoStringArgument(node->GetLeft()) + (node->GetRight()->dType() != DataType::CONST_STR);
    }
    else
    {
        return node->dType() != DataType::CONST_STR;
    }
}


void TranslateScanfMoreParametrs(FILE* fasm, List<variable>* lst, node_t* node, Stubs& stubs, ByteArray& machine_code)
{
    List<node_t*> StackNodePtr; 
    while (node && (node->dType() == DataType::COMMA))
    {
        StackNodePtr.PushBack(node->GetRight());
        node = node->GetLeft();
    }

    uint64_t offset = OffsetVariable(lst, node);
    ScanOne(fasm, offset, stubs, machine_code);  

    for (int i = 0, size = StackNodePtr.Size(); i < size; i++)
    {
        node_t* node = StackNodePtr.ShowBack();
        offset = OffsetVariable(lst, node);
        ScanOne(fasm, offset, stubs, machine_code);
        StackNodePtr.PopBack();
    }
}

void TranslateScanfReturn(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    if (node->GetRight())
    {
        TranslateCallPrintf(fasm, num_const_str, functions, lst, node, funcname, stubs, machine_code);
    }

    TranslateBaseScanf(fasm, stubs, machine_code);

    fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
    fprintf(fasm, "\t\tpush\tqword [buffer]\n");

    machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
    machine_code.Append(BUF);
    machine_code.Append(CMD::PUSH_M64, 3);
    machine_code.Append(BUF);
}


void ScanOne(FILE* fasm, uint64_t offset, Stubs& stubs, ByteArray& machine_code)
{
    TranslateBaseScanf(fasm, stubs, machine_code);
    fprintf(fasm, "\t\tmovsd\tqword [rbp - %ld], xmm0\n", offset);
    AppendMovsdVarXmm0(offset, machine_code);
}

void TranslateBaseScanf(FILE* fasm, Stubs& stubs, ByteArray& machine_code)
{
    fprintf(fasm, "\t\tmov\t\trdx, 31\n");
    fprintf(fasm, "\t\txor\t\trax, rax\n");
    fprintf(fasm, "\t\txor\t\trdi, rdi\n");
    fprintf(fasm, "\t\tmov\t\trsi, str\n");
    fprintf(fasm, "\t\tsyscall\n");
    fprintf(fasm, "\t\tmov\t\trdi, str\n");
    fprintf(fasm, "\t\tcall\tatod\n");

    machine_code.Append(CMD::MOV_RDX_31, 7);
    machine_code.Append(CMD::XOR_RAX_RAX, 3);
    machine_code.Append(CMD::XOR_RDI_RDI, 3);
    AppendMovRsiStr(stubs, machine_code);
    machine_code.Append(CMD::SYSCALL, 2);
    AppendMovRdiStr(stubs, machine_code);
    machine_code.AppendCallFunc("atod");
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

void TranslateRet(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    node = node->GetRight();
    TranslateExp(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
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

void TranslateCallPrintf(FILE* fasm, int& num_const_str,  List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{    
    node = node->GetRight();

    List<node_t*> StackNodePtr; 
    while (node && (node->dType() == DataType::COMMA))
    {
        StackNodePtr.PushBack(node->GetRight());
        node = node->GetLeft();
    }

    if (node->dType() == DataType::CONST_STR)
    {
        PrintString(fasm, num_const_str++, stubs, machine_code);
    }
    else
    {
        TranslateExp(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        PrintOne(fasm, stubs, machine_code);
    }  
    
    for (int i = 0, size = StackNodePtr.Size(); i < size; i++)
    {
        node_t* node = StackNodePtr.ShowBack();
        if (node->dType() == DataType::CONST_STR)
        {
            PrintString(fasm, num_const_str++, stubs, machine_code);
        }
        else
        {
            TranslateExp(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
            PrintOne(fasm, stubs, machine_code);
        }
        StackNodePtr.PopBack();
    }
    PrintCharacter(fasm, '\n', machine_code);
}


void PrintOne(FILE* fasm, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;

    fprintf(fasm, "\t\tmov\t\trdi, str\n");
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
    fprintf(fasm, "\t\tcall\tdtoa\n");
    fprintf(fasm, "\t\tmov\t\trdx, rax\n");
    fprintf(fasm, "\t\tmov\t\trdi, 1\n");
    fprintf(fasm, "\t\tmov\t\trax, 1\n");
    fprintf(fasm, "\t\tmov\t\trsi, str\n");
    fprintf(fasm, "\t\tsyscall\n");

    AppendMovRdiStr(stubs, machine_code);
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
    machine_code.Append(BUF);   
    machine_code.AppendCallFunc("dtoa");
    machine_code.Append(CMD::MOV_RDX_RAX, 3);
    machine_code.Append(CMD::MOV_RAX_1, 7);
    machine_code.Append(CMD::MOV_RDI_1, 7);
    AppendMovRsiStr(stubs, machine_code);
    machine_code.Append(CMD::SYSCALL, 2);

    PrintCharacter(fasm, ' ', machine_code);
}

void PrintCharacter(FILE* fasm, uint32_t character, ByteArray& machine_code)
{
    fprintf(fasm, "\t\tmov\t\trax, %d\n", character);
    fprintf(fasm, "\t\tpush\trax\n");
    fprintf(fasm, "\t\tmov\t\trdx, 1\n");
    fprintf(fasm, "\t\tmov\t\trdi, 1\n");
    fprintf(fasm, "\t\tmov\t\trax, 1\n");
    fprintf(fasm, "\t\tmov\t\trsi, rsp\n");
    fprintf(fasm, "\t\tsyscall\n");
    fprintf(fasm, "\t\tpop\t\trax\n");

    machine_code.Append(CMD::MOV_RAX_CHARACTER, 3);
    machine_code.Append(character);
    machine_code.Append(CMD::PUSH_RAX, 1);
    machine_code.Append(CMD::MOV_RDX_1, 7);
    machine_code.Append(CMD::MOV_RDI_1, 7);
    machine_code.Append(CMD::MOV_RAX_1, 7);
    machine_code.Append(CMD::MOV_RSI_RSP, 3);
    machine_code.Append(CMD::SYSCALL, 2);
    machine_code.Append(CMD::POP_RAX, 1);
}


void PrintString(FILE* fasm, int num_const_str, Stubs& stubs, ByteArray& machine_code)
{
    std::string mark = "str_" + std::to_string(num_const_str);
    const char* mark_c = mark.c_str();
    Label str_lbl = SearchLabel(mark_c, stubs.labels);
    addr_t virtual_addr_mark = str_lbl.Addres();
    addr_t offset_in_file = virtual_addr_mark - stubs.ElfStubs.e_entry;
    uint8_t* bytes = machine_code.ShowMemory(offset_in_file, 4);
    int32_t str_length = 0;
    memcpy(&str_length, bytes, 4);
    addr_t str_vaddr = virtual_addr_mark + sizeof(uint32_t);

    fprintf(fasm, "\t\tmov\t\trsi, str_%d\n", num_const_str);
    fprintf(fasm, "\t\tmov\t\trdi, 1\n");
    fprintf(fasm, "\t\tmov\t\trax, 1\n");
    fprintf(fasm, "\t\tmov\t\trdx, %d\n", str_length);  
    fprintf(fasm, "\t\tsyscall\n");

    machine_code.Append(CMD::MOV_RSI_NUM, 3);
    machine_code.Append((uint32_t)str_vaddr);
    machine_code.Append(CMD::MOV_RDI_1, 7);
    machine_code.Append(CMD::MOV_RAX_1, 7);
    machine_code.Append(CMD::MOV_RDX_NUM, 3);
    machine_code.AppendBin(bytes, 4);
    machine_code.Append(CMD::SYSCALL, 2);

    PrintCharacter(fasm, ' ', machine_code);
}


void TranslateWhile(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    static int num_while = -1;
    if (stubs.reset_while)
    {
        num_while = -1;
        stubs.reset_while = false;
    }
    num_while++;
    int save_num = num_while;
    node_t* condition = node->GetLeft();
    
    char* while_test = new char[20];
    snprintf(while_test, 20, ".while%dtest", save_num);
    char* while_loop = new char[20];
    snprintf(while_loop, 20, ".while%dloop", save_num);
    char* while_end = new char[20];
    snprintf(while_end, 20, ".while%dend", save_num);
    

    fprintf(fasm, "\t\tjmp\t\t%s\n", while_test);
    AppendJmpLabel(while_test, stubs, machine_code);

    fprintf(fasm, ".while%dloop:\n", save_num);
    if (stubs.is_loading)
    {
        Label loop(while_loop, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(loop);
    }

    TranslateOpSequence(fasm, functions, param, node->GetRight(), funcname, offset, stubs, machine_code);

    fprintf(fasm, ".while%dtest:\n", save_num);
    if (stubs.is_loading)
    {
        Label test(while_test, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(test);
    }



    TwoWhileStr mark = {
        .while_loop  = while_loop,
        .while_end   = while_end,
    };

    node_t* last_jmp = TranslateWhileCondSeq(fasm, num_const_str, functions, param, condition, node, mark, funcname, stubs, machine_code);
    if (last_jmp->dType() == DataType::AND)
    {
        fprintf(fasm, "\t\tjmp\t\t%s\n", mark.while_loop);
        AppendJmpLabel(while_loop, stubs, machine_code);     
    }
    else
    {
        //! TODO FIX
        //exit(EXIT_FAILURE);
    }


    fprintf(fasm, ".while%dend:\n", save_num);
    if (stubs.is_loading)
    {
        Label end(while_end, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(end);
    }

    delete[] while_test;
    delete[] while_loop;
}

node_t* TranslateWhileCondSeq(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, node_t* parrent, const TwoWhileStr& strings, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;

    if (node->dType() == DataType::AND || node->dType() == DataType::OR)
    {
        TranslateWhileCondSeq(fasm, num_const_str, functions, param, node->GetLeft(), node, strings, funcname, stubs, machine_code);
        return TranslateWhileCondSeq(fasm, num_const_str, functions, param, node->GetRight(), node, strings, funcname, stubs, machine_code);
    }
    else
    {
        TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
        machine_code.Append(BUF);
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm1, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tcomisd\txmm0, xmm1\n");
        machine_code.Append(CMD::COMISD_XMM0_XMM1, 4);

        const char* jxx = nullptr;
        const char* jmp_mark = nullptr;
        auto AppendLabel = AppendJxxLabel;

        if (parrent->dType() == DataType::AND)
        {
            jxx = Jnx(node);
            jmp_mark = strings.while_end;
            AppendLabel = AppendJnxLabel;
        }
        else
        {
            jxx = Jxx(node);
            jmp_mark = strings.while_loop;
            AppendLabel = AppendJxxLabel;
        }

        fprintf(fasm, "\t\t%s\t\t%s\n", jxx, jmp_mark);
        AppendLabel(jmp_mark, node, stubs, machine_code);
        return parrent;
    }
}




void TranslateIf(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, int offset, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    static int num_if = -1;
    if (stubs.reset_if)
    {
        num_if = -1;
        stubs.reset_if = false;
    }
    num_if++;
    int save_num = num_if;

    node_t* condition = node->GetLeft();

    char* if_else = new char[20];
    snprintf(if_else, 20, ".if%delse", save_num);
    char* if_end = new char[20];
    snprintf(if_end, 20, ".if%dend", save_num);
    char* if_start = new char[20];
    snprintf(if_start, 20, ".if%dstart", save_num);

    ThreeIfStr marks = {
        .if_start = if_start,
        .if_else  = if_else,
        .if_end   = if_end,
    };
    bool have_else = (node->GetRight()->dType() == DataType::ELSE);
    node_t* last_jmp = TranslateIfCondSeq(fasm, num_const_str, functions, param, condition, node, marks, have_else, funcname, stubs, machine_code);
    assert(last_jmp);
    if (last_jmp->dType() == DataType::OR || last_jmp->dType() == DataType::IF)
    {
        const char* if_exit = (have_else) ? if_else : if_end;
        fprintf(fasm, "\t\tjmp\t\t%s\n", if_exit);
        AppendJmpLabel(if_exit, stubs, machine_code);
    }
    else
    {
        // puts("SYKA DAUN");
        // //! TODO FIX
        // exit(EXIT_FAILURE);
    }
    fprintf(fasm, "%s:\n", if_start);
    if (stubs.is_loading)
    {
        Label if_start_lbl(if_start, LabelAddr(stubs, machine_code));
        stubs.labels.PushBack(if_start_lbl);
    }  

    if (have_else)
    {
        node_t* op_else = node->GetRight();
        TranslateOpSequence(fasm, functions, param, op_else->GetLeft(), funcname, offset, stubs, machine_code);

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
            TranslateOpSequence(fasm, functions, param, op_else->GetRight(), funcname, offset, stubs, machine_code);
        }
        else
        {
            TranslateIf(fasm, num_const_str, functions, param, op_else->GetRight(), funcname, offset, stubs, machine_code);
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
        TranslateOpSequence(fasm, functions, param, node->GetRight(), funcname, offset, stubs, machine_code);
        
        fprintf(fasm, "%s:\n", if_end);
        if (stubs.is_loading)
        {
            Label if_end_lbl(if_end, LabelAddr(stubs, machine_code));
            stubs.labels.PushBack(if_end_lbl);
        }  
    }
}

node_t* TranslateIfCondSeq(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, node_t* parrent, const ThreeIfStr& strings, bool have_else, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;

    if (node->dType() == DataType::AND || node->dType() == DataType::OR)
    {
        TranslateIfCondSeq(fasm, num_const_str, functions, param, node->GetLeft(), node, strings, have_else, funcname, stubs, machine_code);
        return TranslateIfCondSeq(fasm, num_const_str, functions, param, node->GetRight(), node, strings, have_else, funcname, stubs, machine_code);
    }
    else
    {
        TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
        machine_code.Append(BUF);
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
        fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
        machine_code.Append(CMD::POP_M64, 3);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tmovsd\txmm1, qword [buffer]\n");
        machine_code.Append(CMD::MOVSD_XMM1_M64, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tcomisd\txmm0, xmm1\n");
        machine_code.Append(CMD::COMISD_XMM0_XMM1, 4);

        const char* if_exit = (have_else) ? strings.if_else : strings.if_end;
        const char* jxx = nullptr;
        const char* jmp_mark = nullptr;
        auto AppendLabel = AppendJxxLabel;

        if (parrent->dType() == DataType::AND)
        {
            jxx = Jnx(node);
            jmp_mark = if_exit;
            AppendLabel = AppendJnxLabel;
        }
        else
        {
            jxx = Jxx(node);
            jmp_mark = strings.if_start;
            AppendLabel = AppendJxxLabel;
        }

        fprintf(fasm, "\t\t%s\t\t%s\n", jxx, jmp_mark);
        AppendLabel(jmp_mark, node, stubs, machine_code);
        return parrent;
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

void TranslateExp(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
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
        TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
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
            TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        }
        else
        {
            fprintf(fasm, "\t\tfldz\n");
            machine_code.Append(CMD::FLDZ, 2);
            fprintf(fasm, "\t\tfstp\tqword [buffer]\n");
            machine_code.Append(CMD::FSTP_M64, 3);
            machine_code.Append(BUF);
            fprintf(fasm, "\t\tpush\tqword [buffer]\n");
            machine_code.Append(CMD::PUSH_M64, 3);
            machine_code.Append(BUF);
        }
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
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
        TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
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
        TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
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
    case DataType::DEG:
        TranslateExp(fasm, num_const_str, functions, param, node->GetLeft(), funcname, stubs, machine_code);
        TranslateExp(fasm, num_const_str, functions, param, node->GetRight(), funcname, stubs, machine_code);
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
        fprintf(fasm, "\t\tcall\t\tpow\n");
        machine_code.AppendCallFunc("pow");
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::CONSTANT:
        static int num_const = 0;
        if (stubs.reset_const_expr)
        {
            num_const = 0;
            stubs.reset_const_expr = false;
        }   
        fprintf(fasm, "\t\tpush\tqword [const_%d]\n", num_const);
        AppendPushConst(num_const++, stubs, machine_code);
        break;
    case DataType::FUNC:
        TranslateCallFunc(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
        machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
        machine_code.Append(BUF);
        fprintf(fasm, "\t\tpush\tqword [buffer]\n");
        machine_code.Append(CMD::PUSH_M64, 3);
        machine_code.Append(BUF);
        break;
    case DataType::SQRT:
        TransateCallSqtr(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;   
    case DataType::SCANF:
        TranslateCallScanf(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::LOG:
        TranslateCallLog(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::SIN:
        TransateCallSin(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;
    case DataType::COS:
        TransateCallCos(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
        break;    
    default:
        break;
    }
}


void TranslateCallLog(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* lst, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    TranslateExp(fasm, num_const_str, functions, lst, node->GetRight(), funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tmovsd\txmm0, qword [buffer]\n");
    machine_code.Append(CMD::MOVSD_XMM0_M64, 5);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tcall\t\tlog10\n");
    machine_code.AppendCallFunc("log10");
    fprintf(fasm, "\t\tmovsd\tqword [buffer], xmm0\n");
    machine_code.Append(CMD::MOVSD_M64_XMM0, 5);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tpush\tqword [buffer]\n");
    machine_code.Append(CMD::PUSH_M64, 3);
    machine_code.Append(BUF); 
}

void TransateCallSin(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    node = node->GetRight();
    TranslateExp(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tfld\tqword [buffer]\n");
    machine_code.Append(CMD::FLD_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tfsin\n");
    machine_code.Append(CMD::FSIN, 2);
    fprintf(fasm, "\t\tfstp\tqword [buffer]\n");
    machine_code.Append(CMD::FSTP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tpush\tqword [buffer]\n");
    machine_code.Append(CMD::PUSH_M64, 3);
    machine_code.Append(BUF);
}

void TransateCallCos(FILE* fasm, int& num_const_str, List<node_t>* functions, List<variable>* param, node_t* node, const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    const uint32_t BUF = stubs.ElfStubs.data_stubs.p_vaddp;
    
    node = node->GetRight();
    TranslateExp(fasm, num_const_str, functions, param, node, funcname, stubs, machine_code);
    fprintf(fasm, "\t\tpop\t\tqword [buffer]\n");
    machine_code.Append(CMD::POP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tfld\tqword [buffer]\n");
    machine_code.Append(CMD::FLD_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tfcos\n");
    machine_code.Append(CMD::FCOS, 2);
    fprintf(fasm, "\t\tfstp\tqword [buffer]\n");
    machine_code.Append(CMD::FSTP_M64, 3);
    machine_code.Append(BUF);
    fprintf(fasm, "\t\tpush\tqword [buffer]\n");
    machine_code.Append(CMD::PUSH_M64, 3);
    machine_code.Append(BUF);
}