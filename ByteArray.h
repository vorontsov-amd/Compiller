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
    const cmd_t MOV_RAX_NUMBER = 0x48c7c0;
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
Label SearchLabel(const std::string& str, List<Label> lst);


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
    Stubs stubs;
    char* array;
    size_t size;
    index_t cur_index;
public:
    ByteArray();
    ByteArray(const char* _array, const size_t _size);
    ~ByteArray();

    index_t Append(const Elf64_Ehdr ehdr);
    index_t Append(const Elf64_Phdr phdr);
    index_t Append(const uint32_t ch);
    index_t Append(const double num);
    index_t Append(const char* func, size_t len);
    void AppendElfHeader();
    void AppendCallFunc(const char* funcname);
    void AppendJmpLabel(const std::string& labelname);
    void AppendJxxLabel(const std::string& labelname, node_t* node);
    void AppendJnxLabel(const std::string& labelname, node_t* node);
    void AppendPushConst(int num_const);
    void AppendStdFunctions();
    void AppendCmd(cmd_t short_cmd, cmd_t long_cmd, int size_cmd, int32_t number);
    index_t AppendCmd(cmd_t cmd, int size_cmd);
    void AppendCmd(cmd_t cmd, uint32_t addr, int size_cmd);
    const char* ShowMemory(size_t offset, size_t length)
    {
        char* memory = new char[length] {0};
        for (int i = 0; i < length; i++)
        {
            memory[i] = array[offset + i];
        }
        return memory;
    }
    const char* ByteCode() const;
    size_t Size() const;
    void Reset();
    addr_t Vaddr() const
    {
        return e_point() + cur_index;
    }
    uint32_t buf() { return stubs.ElfStubs.data_stubs.p_vaddp; }
    uint32_t string_buf() { return stubs.ElfStubs.data_stubs.p_vaddp + sizeof(double); }

    bool stubsNotLoaded() const { return stubs.is_loading; }
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
    bool resetOffset()
    {
        bool status = stubs.reset_init;
        stubs.reset_init = false;
        return status;
    }
    bool resetWhile()
    {
        bool status = stubs.reset_while;
        stubs.reset_while = false;
        return status; 
    }
    bool resetIf()
    {
        bool status = stubs.reset_if;
        stubs.reset_if = false;
        return status;
    }
    bool resetConstExprCounter()
    {
        bool status = stubs.reset_const_expr;
        stubs.reset_const_expr = false; 
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
    auto string_info(std::string str)
    {
        Label str_lbl = SearchLabel(str.c_str(), stubs.labels);
        addr_t virtual_addr_mark = str_lbl.Addres();
        addr_t offset_in_file = virtual_addr_mark - e_point();
        const char* bytes = ShowMemory(offset_in_file, 4);
        uint32_t str_length = 0;
        memcpy(&str_length, bytes, 4);
        addr_t str_vaddr = virtual_addr_mark + sizeof(uint32_t);

        delete[] bytes;
        return std::pair(str_vaddr, str_length);
    }
};