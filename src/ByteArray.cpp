#include "ByteArray.h"
#include "stdlibPath.h"

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

index_t ByteArray::Append(const char* func, size_t len)
{    
    if (cur_index + len >= size - 1)
    {
        size_t new_size = (size + len > 2 * size) ? size + len : 2 * size;
        char* new_array = new char[new_size] {};
        memcpy(new_array, array, size);
        size = new_size;
        delete[] array;
        array = new_array;
    }
    memcpy(array + cur_index, func, len);
    cur_index += len;
    return cur_index;
}

index_t ByteArray::AppendCmd(const cmd_t cmd, int size_cmd)
{
    if (cur_index + size_cmd >= size - 1)
    {
        array = (char*)realloc(array, 2 * size);
        size = 2 * size;
    }
    memcpy(array + cur_index, &cmd, size_cmd);
    memreverse(array + cur_index, size_cmd);
    cur_index += size_cmd;
    return cur_index;
}


void ByteArray::AppendStdFunctions()
{
    FILE* func = fopen(STDLIB_PATH, "rb");
    const size_t FILE_SIZE = 0x5b3;
    const int DTOA_OFFSET = 0x4f;
    const int ATOD_OFFSET = 0x476;
    const int LOG_OFFSET  = 0x31;
    
    try
    {
        char* all = new char[FILE_SIZE] {0};
        fread(all, 1, 0x5b3, func);

        addr_t& pow_addres = stubs.addresses.pow_addres;

        stubs.addresses.pow_addres    = Vaddr();
        stubs.addresses.dtoa_addres   = pow_addres + DTOA_OFFSET;
        stubs.addresses.atod_addres   = pow_addres + ATOD_OFFSET;
        stubs.addresses.log10_adderes = pow_addres + LOG_OFFSET;

        AddStdLabels();
        
        Append(all, FILE_SIZE);

        fclose(func);
        delete[] all;

    }
    catch(const std::bad_alloc& e)
    {
        std::cout << "bad allocated std function\n";
        std::cerr << e.what();
    }
}

const char* ByteArray::ByteCode() const
{
    return array;
}

size_t ByteArray::Size() const
{
    return cur_index;
}

void ByteArray::Reset()
{
    cur_index = 0;
    stubs.is_loading = false;
    stubs.reset_const = true;
    stubs.reset_if = true;
    stubs.reset_while = true;
    stubs.reset_init = true;
    stubs.reset_const_expr = true;
    stubs.reset_const_str = true;
    stubs.reset_const_str_printf = true;
}

void ByteArray::AppendElfHeader()
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

void ByteArray::AppendCmd(cmd_t short_cmd, cmd_t long_cmd, int short_size_cmd, int32_t number)
{
    if (CHAR_MIN <= number && number <= CHAR_MAX)
    {
        uint8_t short_num = number % 0x100;
        cmd_t cmd = short_cmd | short_num;
        AppendCmd(cmd, short_size_cmd);
    }
    else
    {
        cmd_t cmd = long_cmd | number;
        AppendCmd(cmd, short_size_cmd + 3);     
    }
}


void ByteArray::AppendCmd(cmd_t cmd, uint32_t addr, int size_cmd)
{
    AppendCmd(cmd, size_cmd);
    Append(addr);
}


addr_t LabelAddr(Stubs& st, ByteArray& code)
{
    addr_t addr = code.e_point() + code.Size();
    return addr;
}


void ByteArray::AppendCallFunc(const char* funcname)
{
    Label lbl = SearchLabel(funcname, stubs.labels);
    uint32_t distance = lbl.Addres() - Vaddr();

    distance = (distance > 0) ? distance - 5 : distance + 5;

    AppendCmd(CMD::CALL_ADDR, 1);
    Append(distance);
}


void ByteArray::AppendJmpLabel(const std::string& labelname)
{
    Label lbl = SearchLabel(labelname, stubs.labels);

    uint32_t distance = lbl.Addres() - Vaddr();

    distance = (distance > 0) ? distance - 5 : distance + 5;

    AppendCmd(CMD::JMP_ADDR, 1);
    Append(distance);
}


void ByteArray::AppendJxxLabel(const std::string& labelname, node_t* node)
{
    
    Label lbl = SearchLabel(labelname, stubs.labels);
    
    cmd_t jmp = 0;
    switch (node->dType())
    {
    case DataType::JE:
        jmp = CMD::JE_ADDR;
        break;
    case DataType::JNE:
        jmp = CMD::JNE_ADDR;
        break;
    case DataType::JA:
        jmp = CMD::JA_ADDR;
        break;
    case DataType::JAE:
        jmp = CMD::JAE_ADDR;
        break;
    case DataType::JB:
        jmp = CMD::JB_ADDR;
        break;
    case DataType::JBE:
        jmp = CMD::JBE_ADDR;
        break;
    default:
        fprintf(stderr, "Error in condition. Data type isn't condition");
        break;
    }



    uint32_t distance = lbl.Addres() - Vaddr();
    distance = (distance > 0) ? distance - 6 : distance + 6;

    AppendCmd(jmp, 2);
    Append(distance);
}



void ByteArray::AppendJnxLabel(const std::string& labelname, node_t* node)
{
    
    Label lbl = SearchLabel(labelname, stubs.labels);
    
    cmd_t jmp = 0;
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


    uint32_t distance = lbl.Addres() - Vaddr();
    distance = (distance > 0) ? distance - 6 : distance + 6;

    AppendCmd(jmp, 2);
    Append(distance);
}


void ByteArray::AppendPushConst(int num_const)
{
    std::string name = "const_" + std::to_string(num_const);

    Label lbl = SearchLabel(name, stubs.labels);

    AppendCmd(CMD::PUSH_M64, lbl.Addres(), 3);
}


Label SearchLabel(const std::string& funcname, List<Label> lst)
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


