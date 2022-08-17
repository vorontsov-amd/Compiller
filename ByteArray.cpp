#include "ByteArray.h"


//-------------------------- Begin class ByteArray --------------------------

ByteArray::ByteArray()
{
    array = new char[1024] {};

    size = 1024;
    cur_index = 0;
}


ByteArray::ByteArray(const char* _array, const size_t _size)
{
    array = new char[_size] {};
    memcpy(array, _array, _size);
    size = _size;
    cur_index = 0;
}


ByteArray::~ByteArray()
{
    if (array)
    {
        delete[] array;
        array = nullptr;
    }
}


// index_t ByteArray::Append(const char ch)
// {
//     if (cur_index == size - 1)
//     {
//         array = (char*)realloc(array, 2 * size);
//         size = 2 * size;
//     }
//     array[cur_index++] = ch;
//     return cur_index;
// }


index_t ByteArray::Append(const Elf64_Ehdr ehdr)
{
    if (cur_index + sizeof(ehdr) >= size - 1)
    {
        char* new_array = new char[2 * size] {};
        memcpy(new_array, array, size);
        size = 2 * size;
        delete[] array;
        array = new_array;
    }
    memcpy(array + cur_index, &ehdr, sizeof(ehdr));
    cur_index += sizeof(ehdr);
    return cur_index;
}



index_t ByteArray::Append(const Elf64_Phdr phdr)
{
    if (cur_index  + sizeof(phdr) >= size - 1)
    {
        char* new_array = new char[2 * size] {};
        memcpy(new_array, array, size);
        size = 2 * size;
        delete[] array;
        array = new_array;
    }
    memcpy(array + cur_index, &phdr, sizeof(phdr));
    cur_index += sizeof(phdr);
    return cur_index;
}



// index_t ByteArray::Append(const int num)
// {
//     if (cur_index == size - 1)
//     {
//         array = (char*)realloc(array, 2 * size);
//         size = 2 * size;
//     }
//     memcpy(array + cur_index, &num, sizeof(num));
//     cur_index += sizeof(num);
//     return cur_index;
// }



index_t ByteArray::Append(const double num)
{
    
    if (cur_index + sizeof(num) >= size - 1)
    {
        char* new_array = new char[2 * size] {};
        memcpy(new_array, array, size);
        size = 2 * size;
        delete[] array;
        array = new_array;
    }
    memcpy(array + cur_index, &num, sizeof(num));
    cur_index += sizeof(num);
    return cur_index;
}


index_t ByteArray::Append(const addr_t num)
{    
    if (cur_index + sizeof(num) >= size - 1)
    {
        char* new_array = new char[2 * size] {};
        memcpy(new_array, array, size);
        size = 2 * size;
        delete[] array;
        array = new_array;
    }
    memcpy(array + cur_index, &num, sizeof(num));
    cur_index += sizeof(num);
    return cur_index;
}


index_t ByteArray::AppendBin(const uint8_t* func, size_t len)
{    
    LOX

    if (cur_index + len >= size - 1)
    {
        size_t new_size = (size + len > 2 * size) ? size + len : 2 * size;
        char* new_array = new char[new_size] {};
        memcpy(new_array, array, size);
        size = new_size;
        delete[] array;
        array = new_array;
    }
    LOX

    memcpy(array + cur_index, func, len);
    cur_index += len;
    return cur_index;
}


const char* ByteArray::ByteCode() const
{
    return array;
}

size_t ByteArray::Size() const
{
    return cur_index;
}

void ByteArray::Rewind()
{
    cur_index = 0;
}


void  ByteArray::AppendElfHeader()
{
    const int NO_USE = 0;

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
    Append(elf_header);

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
    Append(data_header);

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
    Append(rodata_header);
        
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
    Append(text_header);
}



//--------------------------- End class ByteArray --------------------------

//---------------------------- Begin class Label ----------------------------

addr_t LabelAddr(Stubs& st, ByteArray& code)
{
    addr_t addr = code.e_point() + code.Size();
    return addr;
}


void AppendSubRspNum(uint32_t number, ByteArray& machine_code)
{
    if (number < 0x80)
    {
        uint32_t cmd = CMD::SUB_RSP_NUM_L | number;
        machine_code.Append(cmd, 4);
    }
    else
    {
        uint64_t cmd = CMD::SUB_RSP_NUM_B | number;
        machine_code.Append(cmd, 7);
    }
}


void AppendAddRspNum(uint32_t number, ByteArray& machine_code)
{
    if (number < 0x80)
    {
        uint32_t cmd = CMD::ADD_RSP_NUM_L | number;
        machine_code.Append(cmd, 4);
    }
    else
    {
        uint64_t cmd = CMD::ADD_RSP_NUM_B | number;
        machine_code.Append(cmd, 7);
    }
}


void AppendPushVar(uint32_t number, ByteArray& machine_code)
{
    if (number < 0x80)
    {
        uint8_t offset = -number % 0x100;
        uint32_t cmd = CMD::PUSH_VAR_L | offset;
        machine_code.Append(cmd, 3);
    }
    else
    {
        uint32_t offset = -number;
        uint64_t cmd = CMD::PUSH_VAR_B | offset;
        machine_code.Append(cmd, 6);
    }
}



void AppendMovRaxVarPtr(uint32_t number, ByteArray& machine_code)
{
    if (number < 0x80)
    {
        uint32_t cmd = CMD::MOV_RAX_VAR_L | number;
        machine_code.Append(cmd, 4);
    }
    else
    {
        uint64_t cmd = CMD::MOV_RAX_VAR_B | number;
        machine_code.Append(cmd, 7);
    }
}


void AppendMovsdVarXmm0(uint32_t number, ByteArray& machine_code)
{
    if (number <= 0x80)
    {
        uint8_t offset = -number % 0x100;
        uint64_t cmd = CMD::MOVSD_VAR_XMM0_L | offset;
        machine_code.Append(cmd, 5);
    }
    else
    {
        uint32_t offset = -number;
        uint64_t cmd = CMD::MOVSD_VAR_XMM0_B | offset;
        machine_code.Append(cmd, 8);
    }
}


void AppendMovsdXmm0Var(uint32_t number, ByteArray& machine_code)
{
    if (number <= 0x80)
    {
        uint8_t offset = -number % 0x100;
        uint64_t cmd = CMD::MOVSD_XMM0_VAR_L | offset;
        machine_code.Append(cmd, 5);
    }
    else
    {
        uint32_t offset = -number;
        uint64_t cmd = CMD::MOVSD_XMM0_VAR_B | offset;
        machine_code.Append(cmd, 8);
    }
}


void AppendMovsdXmm0TempVar(uint32_t number, ByteArray& machine_code)
{
    if (number < 0x80)
    {
        uint8_t offset = number;
        uint64_t cmd = CMD::MOVSD_XMM0_VAR_L | offset;
        machine_code.Append(cmd, 5);
    }
    else
    {
        uint32_t offset = number;
        uint64_t cmd = CMD::MOVSD_XMM0_VAR_B | offset;
        machine_code.Append(cmd, 8);
    }
}


void AppendLeaRaxVar(uint32_t number, ByteArray& machine_code)
{
    if (number <= 0x80)
    {
        uint8_t offset = -number % 0x100;
        uint64_t cmd = CMD::LEA_RAX_VAR_L | offset;
        machine_code.Append(cmd, 4);
    }
    else
    {
        uint32_t offset = -number;
        uint64_t cmd = CMD::LEA_RAX_VAR_B | offset;
        machine_code.Append(cmd, 7);
    }
}

void AppendMovRdiStr(Stubs& stubs, ByteArray& machone_code)
{
    uint32_t addr = stubs.ElfStubs.data_stubs.p_vaddp + 8;
    uint64_t cmd = CMD::MOV_RDI_NUM;

    machone_code.Append(cmd, 3);
    machone_code.Append(addr);

}


void AppendMovRsiStr(Stubs& stubs, ByteArray& machone_code)
{
    uint32_t addr = stubs.ElfStubs.data_stubs.p_vaddp + 8;
    uint64_t cmd = CMD::MOV_RSI_NUM;

    machone_code.Append(cmd, 3);
    machone_code.Append(addr);

}


void AppendCallFunc(const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    Label lbl = SearchLabel(funcname, stubs.labels);
    uint32_t distance = lbl.Addres() - machine_code.Vaddr();

    distance = (distance > 0) ? distance - 5 : distance + 5;

    machine_code.Append(CMD::CALL_ADDR, 1);
    machine_code.Append(distance);
}


void AppendJmpLabel(const char* labelname, Stubs& stubs, ByteArray& machine_code)
{
    Label lbl = SearchLabel(labelname, stubs.labels);

    uint32_t distance = lbl.Addres() - machine_code.Vaddr();

    distance = (distance > 0) ? distance - 5 : distance + 5;

    machine_code.Append(CMD::JMP_ADDR, 1);
    machine_code.Append(distance);
}


void AppendJxxLabel(const char* labelname, node_t* node, Stubs& stubs, ByteArray& machine_code)
{
    
    Label lbl = SearchLabel(labelname, stubs.labels);
    
    puts("FUCK");
    uint64_t jmp = 0;
    switch (node->dType())
    {
    case DataType::JE:
            puts("JE");

        jmp = CMD::JE_ADDR;
        break;
    case DataType::JNE:
            puts("JNE");

        jmp = CMD::JNE_ADDR;
        break;
    case DataType::JA:
            puts("JA");

        jmp = CMD::JA_ADDR;
        break;
    case DataType::JAE:
            puts("JAE");

        jmp = CMD::JAE_ADDR;
        break;
    case DataType::JB:
            puts("JB");

        jmp = CMD::JB_ADDR;
        break;
    case DataType::JBE:
        puts("JBE");
        puts(node->value().string_ptr);
        jmp = CMD::JBE_ADDR;
        break;
    default:
        fprintf(stderr, "Error in condition. Data type isn't condition");
        break;
    }



    uint32_t distance = lbl.Addres() - machine_code.Vaddr();
    distance = (distance > 0) ? distance - 6 : distance + 6;

    machine_code.Append(jmp, 2);
    machine_code.Append(distance);
}



void AppendJnxLabel(const char* labelname, node_t* node, Stubs& stubs, ByteArray& machine_code)
{
    
    Label lbl = SearchLabel(labelname, stubs.labels);
    
    uint64_t jmp = 0;
    switch (node->dType())
    {
    case DataType::JE:
        jmp = CMD::JNE_ADDR;
        break;
    case DataType::JNE:
        jmp = CMD::JE_ADDR;
        break;
    case DataType::JA:
        jmp = CMD::JBE_ADDR;
        break;
    case DataType::JAE:
        jmp = CMD::JB_ADDR;
        break;
    case DataType::JB:
        jmp = CMD::JAE_ADDR;
        break;
    case DataType::JBE:
        jmp = CMD::JA_ADDR;
        break;
    default:
        fprintf(stderr, "Error in condition. Data type isn't condition");
        break;
    }


    uint32_t distance = lbl.Addres() - machine_code.Vaddr();
    distance = (distance > 0) ? distance - 6 : distance + 6;

    machine_code.Append(jmp, 2);
    machine_code.Append(distance);
}


void AppendPushConst(int num_const, Stubs& stubs, ByteArray& machine_code)
{
    char name[20] = "";
    snprintf(name, 20, "const_%d", num_const);
    
    Label lbl = SearchLabel(name, stubs.labels);

    machine_code.Append(CMD::PUSH_M64, 3);
    machine_code.Append(lbl.Addres());
}


Label SearchLabel(const char* funcname, List<Label> lst)
{    
    for (int i = 0, size = lst.Size(); i < size; i++)
    {
        if (lst.ShowFront().Contain(funcname))
        {            
            return lst.ShowFront();
        }
        lst.PopFront();
    }
    return Label("NotFound", 0);
}


void* memreverse(void* begin, size_t size)
{
    char temp = 0;
    char* left = (char*)begin;
    char* right = (char*)begin + size - 1;
    for (left, right; left < right; left++, right--)
    {
        temp = *left;
        *left = *right;
        *right = temp;
    }
    return begin;
}


