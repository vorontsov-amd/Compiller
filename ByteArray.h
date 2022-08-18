#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "List.h"
#include "data_t.h"
#include <elf.h>
#include <climits>


typedef unsigned long long index_t;
typedef unsigned int addr_t;
typedef unsigned long long cmd_t;



namespace CMD
{
    const cmd_t FINIT = 0x9bdbe3;
    const cmd_t PUSH_RBP = 0x55;
    const cmd_t PUSH_RAX = 0x50;
    const cmd_t MOV_RBP_RSP = 0x4889e5;
    const cmd_t SUB_RSP_NUM_L = 0x4883ec00;
    const cmd_t SUB_RSP_NUM_B = 0x4881ec00000000;
    const cmd_t ADD_RSP_NUM_L = 0x4883c400;
    const cmd_t ADD_RSP_NUM_B = 0x4881c400000000;
    const cmd_t MOV_RAX_VAR_L = 0x488b4500;
    const cmd_t MOV_RAX_VAR_B = 0x488b8500000000;
    const cmd_t MOVSD_XMM0_VAR = 0xf20f1000;
    const cmd_t MOVSD_VAR_XMM0 = 0xf20f1100;
    const cmd_t MOVSD_VAR_XMM0_L = 0xf20f114500;
    const cmd_t MOVSD_VAR_XMM0_B = 0xf20f118500000000;
    const cmd_t MOVSD_XMM0_VAR_L = 0xf20f104500;
    const cmd_t MOVSD_XMM0_VAR_B = 0xf20f108500000000;
    const cmd_t LEA_RAX_VAR_L = 0x488d4500;
    const cmd_t LEA_RAX_VAR_B = 0x488d8500000000;
    const cmd_t CALL_ADDR = 0xe8;
    const cmd_t JMP_ADDR = 0xe9;
    const cmd_t JE_ADDR = 0x0f84;
    const cmd_t JNE_ADDR = 0x0f85;
    const cmd_t JA_ADDR = 0x0f87;
    const cmd_t JAE_ADDR = 0x0f83;
    const cmd_t JB_ADDR = 0x0f82;
    const cmd_t JBE_ADDR = 0x0f86;
    const cmd_t POP_M64 = 0x8f0425;
    const cmd_t PUSH_M64 = 0xff3425;
    const cmd_t PUSH_VAR_L = 0xff7500;
    const cmd_t PUSH_VAR_B = 0xffb500000000;
    const cmd_t SQRTSD_XMM0_M64 = 0xf20f510425;
    const cmd_t MOVSD_M64_XMM0 = 0xf20f110425;
    const cmd_t MOVSD_XMM0_M64 = 0xf20f100425;
    const cmd_t MOVSD_XMM1_M64 = 0xf20f100c25;
    const cmd_t ADDSD_XMM0_XMM1 = 0xf20f58c1;
    const cmd_t SUBSD_XMM0_XMM1 = 0xf20f5cc1;
    const cmd_t MULSD_XMM0_XMM1 = 0xf20f59c1;
    const cmd_t DIVSD_XMM0_XMM1 = 0xf20f5ec1;
    const cmd_t COMISD_XMM0_XMM1 = 0x660f2fc1;
    const cmd_t LEAVE = 0xc9;
    const cmd_t RET = 0xc3;
    const cmd_t FLDZ = 0xd9ee;
    const cmd_t FSTP_M64 = 0xdd1c25;
    const cmd_t MOV_RAX_60 = 0x48c7c03c000000;
    const cmd_t XOR_RDI_RDI = 0x4831ff;
    const cmd_t SYSCALL = 0x0f05;
    const cmd_t MOV_RDI_NUM = 0x48c7c7;
    const cmd_t MOV_RSI_NUM = 0x48c7c6;
    const cmd_t MOV_RDX_RAX = 0x4889c2;
    const cmd_t MOV_RDI_1 = 0x48c7c701000000;
    const cmd_t MOV_RAX_1 = 0x48c7c001000000;
    const cmd_t MOV_RDX_31 = 0x48c7c21f000000;
    const cmd_t XOR_RAX_RAX = 0x4831c0;
    const cmd_t INC_RDX = 0x48ffc2;
    const cmd_t MOV_RAX_CHARACTER = 0x48c7c0;
    const cmd_t MOV_RDX_1 = 0x48c7c201000000;
    const cmd_t MOV_RSI_RSP = 0x4889e6;
    const cmd_t POP_RAX = 0x58;
    const cmd_t MOV_RDX_NUM = 0x48c7c2;
    const cmd_t MOVQ_RAX_XMM0 = 0x66480f7ec0;
    const cmd_t FLD_M64 = 0xdd0425;
    const cmd_t FSIN = 0xd9fe;
    const cmd_t FCOS = 0xd9ff;
};




void* memreverse(void* begin, size_t size);

struct Stubs;
class ByteArray;
class Label;

addr_t LabelAddr(Stubs& sturb, ByteArray& code);
Label SearchLabel(const char* funcname, List<Label> lst);


class Label
{
private:
    std::string name;
    addr_t addr;
public:
    Label() : addr(0) {}
    Label(const Label& lbl) : name(lbl.name), addr(lbl.addr) {}
    Label(std::string _name, const addr_t _addr) : name(_name), addr(_addr) {}
    ~Label() {}

    friend std::ostream& operator<< (std::ostream& stream, Label const& lbl)
    {
        stream << "name: " << lbl.name << "\nptr: " << lbl.addr << "\n";
        return stream;
    }


    bool Contain(const std::string str) const { return name == str; };
    addr_t Addres() const { return addr; };
};


struct PhdrStubs
{
    addr_t p_offset;
    addr_t p_vaddp;
    addr_t p_size;
};


struct EhdrStubs
{
    addr_t e_entry = 0x30200000;
    PhdrStubs rodata_stubs;
    PhdrStubs data_stubs;
    PhdrStubs text_stubs;
};


struct Addresses
{
    addr_t pow_addres;
    addr_t log10_adderes;
    addr_t dtoa_addres;
    addr_t atod_addres;   
};


struct Stubs
{
    List<Label> labels;
    EhdrStubs ElfStubs;
    Addresses addresses;

    bool is_loading = true;
    bool reset_if = false;
    bool reset_const = false;
    bool reset_const_expr = false;
    bool reset_const_str = false;
    bool reset_const_str_printf = false;
    bool reset_while = false;
    bool reset_init  = false;
};


class ByteArray
{
private:
    char* array;
    size_t size;
    index_t cur_index;
public:
    Stubs stubs;
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
    void AppendElfHeader();
    void AppendCallFunc(const char* funcname);
    void AppendStdFunctions();
    void AppendCmd(cmd_t short_cmd, cmd_t long_cmd, int size_cmd, int32_t number);
    index_t AppendCmd(cmd_t cmd, int size_cmd);



    //index_t Append(const uint64_t num);
    index_t Append(const cmd_t cmd, int size_cmd);
    index_t AppendBin(const uint8_t* cmd, size_t size);

    uint8_t* ShowMemory(size_t offset, size_t length)
    {
        uint8_t* memory = new uint8_t[length] {0};
        for (int i = 0; i < length; i++)
        {
            memory[i] = array[offset + i];
        }
        return memory;
    }

    const char* ByteCode() const;
    size_t Size() const;
    void Rewind();
    addr_t Vaddr() const
    {
        return e_point() + cur_index;
    }
    
    bool stubsNotLoaded() { return stubs.is_loading; }
    bool resetConstDeclCounter() 
    {  
        bool status = stubs.reset_const;
        stubs.reset_const = false; 
        return status; 
    }
    bool resetStrDeclCounter()
    {
        bool status = stubs.reset_const_str;
        stubs.reset_const = false; 
        return status; 
    }
    bool resetStrPrintCounter()
    {
        bool status = stubs.reset_const_str_printf;
        stubs.reset_const_str_printf = false; 
        return status;         
    }
    addr_t e_point() const { return stubs.ElfStubs.e_entry; }
    PhdrStubs& dataStubs() { return stubs.ElfStubs.data_stubs; }
    PhdrStubs& rodataStubs() { return stubs.ElfStubs.rodata_stubs; }
    PhdrStubs& textStubs() { return stubs.ElfStubs.text_stubs; }
    Addresses& addrs() { return stubs.addresses; }
    void AddLabel(const std::string& lbl_name) { stubs.labels.PushBack(Label(lbl_name, Vaddr())); }
    void AddLabel(const std::string& lbl_name, addr_t addr) { stubs.labels.PushBack(Label(lbl_name, addr)); }
    void AddStdLabels() 
    {
        AddLabel("dtoa", stubs.addresses.dtoa_addres);
        AddLabel("pow", stubs.addresses.pow_addres);
        AddLabel("atod", stubs.addresses.atod_addres);
        AddLabel("log10", stubs.addresses.log10_adderes);
    }
};





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