#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "List.h"
#include "data_t.h"
#include <elf.h>


typedef unsigned long long index_t;
typedef unsigned int addr_t;
typedef const unsigned long long cmd_t;
typedef const unsigned int correct_t; 



namespace CMD
{
    cmd_t FINIT = 0x9bdbe3;
    cmd_t PUSH_RBP = 0x55;
    cmd_t PUSH_RAX = 0x50;
    cmd_t MOV_RBP_RSP = 0x4889e5;
    cmd_t SUB_RSP_NUM_L = 0x4883ec00;
    cmd_t SUB_RSP_NUM_B = 0x4881ec00000000;
    cmd_t ADD_RSP_NUM_L = 0x4883c400;
    cmd_t ADD_RSP_NUM_B = 0x4881c400000000;
    cmd_t MOV_RAX_VAR_L = 0x488b4500;
    cmd_t MOV_RAX_VAR_B = 0x488b8500000000;
    cmd_t MOVSD_XMM0_VAR = 0xf20f1000;
    cmd_t MOVSD_VAR_XMM0 = 0xf20f1100;
    cmd_t MOVSD_VAR_XMM0_L = 0xf20f114500;
    cmd_t MOVSD_VAR_XMM0_B = 0xf20f118500000000;
    cmd_t MOVSD_XMM0_VAR_L = 0xf20f104500;
    cmd_t MOVSD_XMM0_VAR_B = 0xf20f108500000000;
    cmd_t LEA_RAX_VAR_L = 0x488d4500;
    cmd_t LEA_RAX_VAR_B = 0x488d8500000000;
    cmd_t CALL_ADDR = 0xe8;
    cmd_t JMP_ADDR = 0xe9;
    cmd_t JE_ADDR = 0x0f84;
    cmd_t JNE_ADDR = 0x0f85;
    cmd_t JA_ADDR = 0x0f87;
    cmd_t JAE_ADDR = 0x0f83;
    cmd_t JB_ADDR = 0x0f82;
    cmd_t JBE_ADDR = 0x0f86;
    cmd_t POP_M64 = 0x8f0425;
    cmd_t PUSH_M64 = 0xff3425;
    cmd_t PUSH_VAR_L = 0xff7500;
    cmd_t PUSH_VAR_B = 0xffb500000000;
    cmd_t SQRTSD_XMM0_M64 = 0xf20f510425;
    cmd_t MOVSD_M64_XMM0 = 0xf20f110425;
    cmd_t MOVSD_XMM0_M64 = 0xf20f100425;
    cmd_t MOVSD_XMM1_M64 = 0xf20f100c25;
    cmd_t ADDSD_XMM0_XMM1 = 0xf20f58c1;
    cmd_t SUBSD_XMM0_XMM1 = 0xf20f5cc1;
    cmd_t MULSD_XMM0_XMM1 = 0xf20f59c1;
    cmd_t DIVSD_XMM0_XMM1 = 0xf20f5ec1;
    cmd_t COMISD_XMM0_XMM1 = 0x660f2fc1;
    cmd_t LEAVE = 0xc9;
    cmd_t RET = 0xc3;
    cmd_t FLDZ = 0xd9ee;
    cmd_t FSTP_M64 = 0xdd1c25;
    cmd_t MOV_RAX_60 = 0x48c7c03c000000;
    cmd_t XOR_RDI_RDI = 0x4831ff;
    cmd_t SYSCALL = 0x0f05;
    cmd_t MOV_RDI_NUM = 0x48c7c7;
    cmd_t MOV_RSI_NUM = 0x48c7c6;
    cmd_t MOV_RDX_RAX = 0x4889c2;
    cmd_t MOV_RDI_1 = 0x48c7c701000000;
    cmd_t MOV_RAX_1 = 0x48c7c001000000;
    cmd_t MOV_RDX_31 = 0x48c7c21f000000;
    cmd_t XOR_RAX_RAX = 0x4831c0;
    cmd_t INC_RDX = 0x48ffc2;
    cmd_t MOV_RAX_CHARACTER = 0x48c7c0;
    cmd_t MOV_RDX_1 = 0x48c7c201000000;
    cmd_t MOV_RSI_RSP = 0x4889e6;
    cmd_t POP_RAX = 0x58;
    
    correct_t CALL_CORRECT = -0x5;
};




void* memreverse(void* begin, size_t size);


class ByteArray
{
private:
    const addr_t e_point = 0x30200000;
    char* array;
    size_t size;
    index_t cur_index;
public:
    ByteArray();
    ByteArray(const char* _array, const size_t _size);
    ~ByteArray();

    //index_t Append(const char ch);
    index_t Append(const Elf64_Ehdr ehdr);
    index_t Append(const Elf64_Phdr phdr);
    index_t Append(const uint32_t ch);
    index_t Append(const uint8_t num);
    index_t Append(const double num);
    index_t Append(const char* func, size_t len);
    //index_t Append(const uint64_t num);
    template <typename T> index_t Append(const T cmd, size_t size_cmd);
    index_t AppendBin(const uint8_t* cmd, size_t size);


    const char* ByteCode() const;
    size_t Size() const;
    void Rewind();
    addr_t Vaddr() const
    {
        return e_point + cur_index;
    }
};


template <typename T> index_t ByteArray::Append(const T cmd, size_t size_cmd)
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



class Label
{
private:
    char* name;
    addr_t addr;
public:
    Label();
    Label(const Label& lbl);
    Label(const char* _name, const addr_t _addr);
    ~Label();

    friend std::ostream& operator<< (std::ostream& stream, Label const& lbl)
    {
        stream << "name: " << lbl.name << "\nptr: " << lbl.addr << "\n";
        return stream;
    }

    Label& operator=(const Label& data);

    bool Contain(const char* str) const;
    addr_t Addres() const;
};


struct PhdrStubs
{
    addr_t p_offset;
    addr_t p_vaddp;
    addr_t p_size;
};


struct EhdrStubs
{
    addr_t e_entry;
    PhdrStubs rodata_stubs;
    PhdrStubs data_stubs;
    PhdrStubs text_stubs;
};


struct Stubs
{
    List<Label> labels;
    EhdrStubs ElfStubs;
    addr_t pow_addres;
    addr_t log10_adderes;
    addr_t isnan_addres;
    addr_t isinf_addres;
    addr_t strlen_addres;
    addr_t floor_addres;
    addr_t dtoa_addres;
    addr_t atod_addres;
    bool is_loading = true;
    bool rewind_if = false;
    bool rewind_const = false;
    bool rewind_const_expr = false;
    bool rewind_while = false;
    bool rewind_init = false;
};



addr_t LabelAddr(Stubs& st, ByteArray& code);
Label SearchLabel(const char* funcname, List<Label> lst);

void AppendCallFunc(const char* funcname, Stubs& stubs, ByteArray& machine_code);
void AppendJmpLabel(const char* labelname, Stubs& stubs, ByteArray& machine_code);
void AppendJxxLabel(const char* labelname, node_t* node, Stubs& stubs, ByteArray& machine_code);
void AppendJnxLabel(const char* labelname, node_t* node, Stubs& stubs, ByteArray& machine_code);
void AppendPushConst(int num_const, Stubs& stubs, ByteArray& machine_code);
void AppendSubRspNum(uint32_t number, ByteArray& machine_code);
void AppendAddRspNum(uint32_t number, ByteArray& machine_code);
void AppendMovRaxVarPtr(uint32_t number, ByteArray& machine_code);
void AppendMovsdVarXmm0(uint32_t number, ByteArray& machine_code);
void AppendMovsdXmm0Var(uint32_t number, ByteArray& machine_code);
void AppendMovsdXmm0TempVar(uint32_t number, ByteArray& machine_code);
void AppendLeaRaxVar(uint32_t number, ByteArray& machine_code);
void AppendPushVar(uint32_t number, ByteArray& machine_code);
void AppendMovRdiStr(Stubs& stubs, ByteArray& machone_code);
void AppendMovRsiStr(Stubs& stubs, ByteArray& machone_code);