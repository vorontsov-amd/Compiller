#include "Differenciator.h"
#include "List.h"

#define CHECK_STR_PTR(STR)                                                                                  \
	if ((STR) == nullptr)                                                                                    \
    {                                                                                                         \
        fprintf(stderr, "NULL STRING PTR in function %s, line %d\n", __PRETTY_FUNCTION__, __LINE__);           \
    }                               


node_t& GetGrammar(List<node_t>& programm);
node_t* GetOpSequence(List<node_t>& programm);
node_t* GetOperator(List<node_t>& programm);
node_t* GetVar(List<node_t>& programm);
node_t* GetExpression(List<node_t>& programm);
node_t* GetT(List<node_t>& programm);
node_t* GetPrimaryExpression(List<node_t>& programm);
node_t* GetNumber(List<node_t>& programm);
node_t* GetPower(List<node_t>& programm);
node_t* GetIfWhile(List<node_t>& programm);


