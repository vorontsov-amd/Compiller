#pragma once
#include "Differenciator.h"
#include "List.h"

#define CHECK_STR_PTR(STR)                                                                                  \
	if ((STR) == nullptr)                                                                                    \
    {                                                                                                         \
        fprintf(stderr, "NULL STRING PTR in function %s, line %d\n", __PRETTY_FUNCTION__, __LINE__);           \
        exit(EXIT_FAILURE);                                                                                     \
    }                               


List<DifferTree> GetGrammar(List<node_t>& programm);
node_t* GetOpSequence(List<node_t>& programm);
node_t* GetOperator(List<node_t>& programm);
node_t* GetVar(List<node_t>& programm);
node_t* GetExpression(List<node_t>& programm);
node_t* GetTerm(List<node_t>& programm);
node_t* GetPrimaryExpression(List<node_t>& programm);
node_t* GetNumber(List<node_t>& programm);
node_t* GetPower(List<node_t>& programm);
node_t* GetIf(List<node_t>& programm);
node_t* GetWhile(List<node_t>& programm);
node_t* GetElse(List<node_t>& programm);
node_t* GetAssign(List<node_t>& programm);
void CheckClsRoundBr(List<node_t>& programm);
void CheckOpRoundBr(List<node_t>& programm);
node_t* GetConditions(List<node_t>& programm);
node_t* GetInit(List<node_t>& programm);
void CheckWord(List<node_t>& programm);
node_t* GetRetFunc(List<node_t>& programm);
node_t* GetCallFunc(List<node_t>& programm);
node_t* GetFunc(List<node_t>& programm, node_t& func);
node_t& GetDefFunc(List<node_t>& programm);
char* Funcname(node_t& func);
node_t* GetParamSequence(List<node_t>& programm);
void CheckValidFunc(List<node_t>& programm);
void CheckClsShapeBr(List<node_t>& programm);
void CheckOpShapeBr(List<node_t>& programm);
node_t* GetNewVar(List<node_t>& programm);
node_t* GetReturn(List<node_t>& programm);

