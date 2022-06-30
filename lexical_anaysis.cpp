#include "lexical_analysis.h"

List<node_t> AnalysProcessing (char* programm, long long length)
{
    CHECK_STR_PTR(programm);

    List<node_t> lexems;

    char* word_ptr = new char[length];
    for (int str_len = 0; (str_len = sscanf(programm, "%s",word_ptr)) != -1; programm += (strlen(word_ptr) + 1))
    {
        WordAnalysis(word_ptr, lexems);
    }
    node_t term(NodeType::TERMINATED);
    lexems.PushBack(term);
    delete[] word_ptr;
    return lexems;
}


void WordAnalysis(char* word_ptr, List<node_t>& lexems)
{
    CHECK_STR_PTR(word_ptr);

    if (isNumber(word_ptr))
    {
        node_t number(NodeType::NUMBER, DataType::CONSTANT, atof(word_ptr)); 
        lexems.PushBack(number);
    }
    else if (isWord(word_ptr))
    {
        node_t word(NodeType::WORD, DataType::UNKNOWN, word_ptr);
        lexems.PushBack(word);
    }
    else if (symbol* op = isOperator(word_ptr))
    {
        node_t oper(NodeType::OPERATOR, op->type, op->ch);
        lexems.PushBack(oper);
    }
    else if (symbol* br = isBracket(word_ptr))
    {
        node_t bracket(NodeType::BRACKET, br->type, br->ch);
        lexems.PushBack(bracket);
    }
    else 
    {        
        if (char* ch = isSubstring(word_ptr))
        {  
            SubstringAnalysis(word_ptr, ch, lexems);
        }
        else
        {
            node_t word_with_number(NodeType::WORD_WITH_NUMBERS, DataType::UNKNOWN, word_ptr);
            lexems.PushBack(word_with_number);
        }
    }
}


#define STRSTR(S) if (ch = strstr(str, (S))) { return ch; }

char* isSubstring(char* str)
{
    CHECK_STR_PTR(str);
    
    char* ch = nullptr;
    STRSTR(";")
    else STRSTR("=")
    else STRSTR("+")
    else STRSTR("-")
    else STRSTR("*")
    else STRSTR("/")
    else STRSTR("^")
    else STRSTR("<")
    else STRSTR(">")
    else STRSTR("<=")
    else STRSTR(">=")
    else STRSTR("==")
    else STRSTR("!=")
    else STRSTR("{")
    else STRSTR("}")
    else STRSTR("(")
    else STRSTR(")")
    else STRSTR("[")
    else STRSTR("]")
    else STRSTR(",")
    return nullptr;
}

#undef STRSTR

void SubstringAnalysis(char* word_ptr, char* ch, List<node_t>& lexems)
{
    CHECK_STR_PTR(word_ptr);
    CHECK_STR_PTR(ch);
    
    char smb = *ch;
    *ch = '\0';
    if (word_ptr[0] != '\0') WordAnalysis(word_ptr, lexems);
    *ch = smb;
    if (ch[1] == '=')
    {
        smb = ch[2];
        ch[2] = '\0';
        WordAnalysis(ch, lexems);
        ch[2] = smb;
        if (ch[2] != '\0') WordAnalysis(&ch[2], lexems); 
    }
    else
    {
        smb = ch[1];
        ch[1] = '\0';
        WordAnalysis(ch, lexems);
        ch[1] = smb;
        if (ch[1] != '\0') WordAnalysis(&ch[1], lexems); 
    }
}


bool isNumber(char* str)
{
    CHECK_STR_PTR(str);
    
    int status = 0, i = 0;
    for (i; *str != '\0'; i++, str++)
    {
        status += std::isdigit(*str);
    }
    
    return status == i;
}


bool isWord(char* str)
{
    CHECK_STR_PTR(str);

    int status = 0, i = 0;
    for (i; *str != '\0'; i++, str++)
    {
        status += (bool)std::isalpha(*str);
    }
    
    return status == i;
}

symbol* isOperator(char* str)
{
    CHECK_STR_PTR(str);

    char operators[14][3] = {"+", "-", "=", "*", "<", ">", "/", "^", ">=", "<=", "!=", "==", ";", ","};
    int result =  SearchOperator(operators, 14, str);
    if (result != -1)
    {
        symbol* op = new symbol;
        switch (result)
        {
        case 0:
            op->type = DataType::ADD;
            break;
        case 1:
            op->type = DataType::SUB;
            break;
        case 2:
            op->type = DataType::MOV;
            break;
        case 3:
            op->type = DataType::MUL;
            break;
        case 4:
            op->type = DataType::JB;
            break;
        case 5:
            op->type = DataType::JA;
            break;
        case 6:
            op->type = DataType::DIV;
            break;
        case 7:
            op->type = DataType::DEG;
            break;
        case 8:
            op->type = DataType::JAE;
            break;
        case 9:
            op->type = DataType::JBE;
            break;
        case 10:
            op->type = DataType::JNE;
            break;
        case 11:
            op->type = DataType::JE;
            break;
        case 12:
            op->type = DataType::END_OP;
            break;
        case 13:
            op->type = DataType::COMMA;
            break;
        }
        op->ch = new char[3];
        strcpy(op->ch, operators[result]);
        return op;
    }
    return nullptr;
}

template <unsigned N>
int SearchOperator(char array[][N], int length, char* element)
{
    CHECK_STR_PTR(array);
    CHECK_STR_PTR(element);
    
    int number = -1;
    for (int i = 0; i < length; i++)
    {
        if (strcmp(array[i], element) == 0)
        {
            number = i;
        }
    }
    return number;
}

symbol* isBracket(char* str)
{
    CHECK_STR_PTR(str);

    char operators[6][2] = {"{", "}", "[", "]", "(", ")"};
    int result =  SearchOperator(operators, 6, str);
    if (result != -1)
    {
        symbol* br = new symbol;
        switch (result)
        {
        case 0:
            br->type = DataType::OP_SHAPE_BR;
            break;
        case 1:
            br->type = DataType::CLS_SHAPE_BR;
            break;
        case 2:
            br->type = DataType::OP_SQUARE_BR;
            break;
        case 4:
            br->type = DataType::OP_ROUND_BR;
            break;
        case 5:
            br->type = DataType::CLS_ROUND_BR;
            break;
        }
        br->ch = new char[2];
        strcpy(br->ch, operators[result]);
        return br;
    }
    return nullptr;
}