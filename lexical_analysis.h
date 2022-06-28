#include "List.h"
#include "data_t.h"

#define CHECK_STR_PTR(STR)                                                                                  \
	if ((STR) == nullptr)                                                                                    \
    {                                                                                                         \
        fprintf(stderr, "NULL STRING PTR in function %s, line %d\n", __PRETTY_FUNCTION__, __LINE__);           \
    }  


struct symbol
{
    DataType::dataType type;
    char* ch;
};



bool isNumber(char* str);
bool isWord(char* str);
symbol* isOperator(char* str);
bool isSemicolon(char* str);

template <unsigned N> int SearchOperator(char array[][N], int length, char* element);
symbol* isBracket(char* str);
List<node_t> AnalysProcessing (char* programm, long long length);