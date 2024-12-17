#pragma once
#include "data_t.h"
#include <deque>

#define CHECK_STR_PTR(STR)                                                                                  \
	if ((STR) == nullptr)                                                                                    \
    {                                                                                                         \
        fprintf(stderr, "NULL STRING PTR in function %s, line %d\n", __PRETTY_FUNCTION__, __LINE__);           \
        exit(EXIT_FAILURE);                                                                                     \
    }       

struct symbol
{
    DataType::dataType type;
    char* ch;
    symbol()
    {
        type = DataType::UNKNOWN;
        ch = nullptr;
    }
    ~symbol()
    {
        delete[] ch;
        ch = nullptr;
    }
};



bool isNumber(char* str);
bool isWord(char* str);
symbol* isOperator(char* str);

template <unsigned N> int SearchOperator(char array[][N], int length, char* element);
symbol* isBracket(char* str);
std::deque<node_t> AnalysProcessing (char* programm, long long length);
void SubstringAnalysis(char* word_ptr, char* ch, std::deque<node_t>& lexems);
void WordAnalysis(char* word_ptr, std::deque<node_t>& lexems);
char* isSubstring(char* str);