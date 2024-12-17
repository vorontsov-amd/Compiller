#pragma once
#include "Differentiator.h"
#include <vector>
#include <deque>

#define CHECK_STR_PTR(STR)                                                                                  \
	if ((STR) == nullptr)                                                                                    \
    {                                                                                                         \
        fprintf(stderr, "NULL STRING PTR in function %s, line %d\n", __PRETTY_FUNCTION__, __LINE__);           \
        exit(EXIT_FAILURE);                                                                                     \
    }                               


std::vector<DifferTree*> GetGrammar(std::deque<node_t>& programm);
node_t* GetOpSequence(std::deque<node_t>& programm);
node_t* GetOperator(std::deque<node_t>& programm);
node_t* GetVar(std::deque<node_t>& programm);
node_t* GetExpression(std::deque<node_t>& programm);
node_t* GetTerm(std::deque<node_t>& programm);
node_t* GetPrimaryExpression(std::deque<node_t>& programm);
node_t* GetNumber(std::deque<node_t>& programm);
node_t* GetPower(std::deque<node_t>& programm);
node_t* GetIf(std::deque<node_t>& programm);
node_t* GetWhile(std::deque<node_t>& programm);
node_t* GetElse(std::deque<node_t>& programm);
node_t* GetAssign(std::deque<node_t>& programm);
void CheckClsRoundBr(std::deque<node_t>& programm);
void CheckOpRoundBr(std::deque<node_t>& programm);
node_t* GetConditions(std::deque<node_t>& programm);
node_t* GetInit(std::deque<node_t>& programm);
void CheckWord(std::deque<node_t>& programm);
node_t* GetRetFunc(std::deque<node_t>& programm);
node_t* GetCallFunc(std::deque<node_t>& programm);
node_t* GetFunc(std::deque<node_t>& programm, node_t& func);
node_t& GetDefFunc(std::deque<node_t>& programm);
char* Funcname(node_t& func);
node_t* GetParamSequence(std::deque<node_t>& programm);
node_t* GetArgumentSequence(std::deque<node_t>& programm, bool& no_string);
void CheckValidFunc(std::deque<node_t>& programm);
void CheckClsShapeBr(std::deque<node_t>& programm);
void CheckOpShapeBr(std::deque<node_t>& programm);
node_t* GetNewVar(std::deque<node_t>& programm);
node_t* GetReturn(std::deque<node_t>& programm);
node_t* GetStr(std::deque<node_t>& programm);
node_t* GetPrimaryCondExpression(std::deque<node_t>& programm);
node_t* GetCondTerm(std::deque<node_t>& programm);
node_t* GetCondExpression(std::deque<node_t>& programm);

