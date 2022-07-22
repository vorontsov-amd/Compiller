#include "ByteArray.h"


//-------------------------- Begin class ByteArray --------------------------

ByteArray::ByteArray()
{
    array = (char*)calloc(1024, 1);
    size = 1024;
    cur_index = 0;
}


ByteArray::ByteArray(const char* _array, const size_t _size)
{
    array = (char*)calloc(_size, 1);
    memcpy(array, _array, _size);
    size = _size;
    cur_index = 0;
}


ByteArray::~ByteArray()
{
    if (array) free(array);
    array = nullptr;
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
    if (cur_index == size - 1)
    {
        array = (char*)realloc(array, 2 * size);
        size = 2 * size;
    }
    memcpy(array + cur_index, &ehdr, sizeof(ehdr));
    cur_index += sizeof(ehdr);
    return cur_index;
}



index_t ByteArray::Append(const Elf64_Phdr phdr)
{
    if (cur_index == size - 1)
    {
        array = (char*)realloc(array, 2 * size);
        size = 2 * size;
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
    if (cur_index == size - 1)
    {
        array = (char*)realloc(array, 2 * size);
        size = 2 * size;
    }
    memcpy(array + cur_index, &num, sizeof(num));
    cur_index += sizeof(num);
    return cur_index;
}


index_t ByteArray::Append(const addr_t num)
{    
    if (cur_index == size - 1)
    {
        array = (char*)realloc(array, 2 * size);
        size = 2 * size;
    }
    memcpy(array + cur_index, &num, sizeof(num));
    cur_index += sizeof(num);
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

//--------------------------- End class ByteArray --------------------------

//---------------------------- Begin class Label ----------------------------

Label::Label()
{
    name = nullptr;
    addr = 0;
}

Label::Label(const Label& lbl)
{
    name = new char[strlen(lbl.name) + 1] {0};
    strcpy(name, lbl.name);
    addr = lbl.addr;
}

Label::Label(const char* _name, const addr_t _addr)
{
    name = new char[strlen(_name) + 1] {0};
    strcpy(name, _name);
    addr = _addr;
}

Label::~Label()
{

}

bool Label::Contain(const char* str) const
{
    return strcmp(str, name) == 0;
}

addr_t Label::Addres() const
{
    return addr;
}


Label& Label::operator=(const Label& lbl)
{
    if (name) 
    {
        delete[] name;
    }
    if (lbl.name)
    {
        name = new char[strlen(lbl.name) + 1];
        strcpy(name, lbl.name);
    }
    else
    {
        name = nullptr;
    }

    addr = lbl.addr;
    return *this;
}


//----------------------------- End class Label -----------------------------


addr_t LabelAddr(Stubs& st, ByteArray& code)
{
    addr_t addr = st.ElfStubs.e_entry + code.Size();
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
        uint32_t cmd = CMD::PUSH_VAR_L | number;
        machine_code.Append(cmd, 4);
    }
    else
    {
        uint64_t cmd = CMD::PUSH_VAR_B | number;
        machine_code.Append(cmd, 7);
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


void AppendCallFunc(const char* funcname, Stubs& stubs, ByteArray& machine_code)
{
    Label lbl = SearchLabel(funcname, stubs.labels);
    uint32_t distance = lbl.Addres() - machine_code.Vaddr() - 5;

    machine_code.Append(CMD::CALL_ADDR, 1);
    machine_code.Append(distance);
}


void AppendJmpLabel(const char* labelname, Stubs& stubs, ByteArray& machine_code)
{
    Label lbl = SearchLabel(labelname, stubs.labels);


    uint32_t distance = lbl.Addres() - machine_code.Vaddr() - 5;

    machine_code.Append(CMD::JMP_ADDR, 1);
    machine_code.Append(distance);
}


void AppendJxxLabel(const char* labelname, node_t* node, Stubs& stubs, ByteArray& machine_code)
{
    
    Label lbl = SearchLabel(labelname, stubs.labels);
    
    uint64_t jmp = 0;
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



    uint32_t distance = lbl.Addres() - machine_code.Vaddr() + 1;

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

    uint32_t distance = lbl.Addres() - machine_code.Vaddr() - 6;

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


void memreverse(void* begin, size_t size)
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
}


