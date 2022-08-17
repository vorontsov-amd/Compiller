#include "Reading.h"


List<DifferTree> GetGrammar(List<node_t>& programm)
{    
	List<DifferTree> project;
	while (programm.ShowFront().Type() != NodeType::TERMINATED)
	{
		DifferTree* tree = new DifferTree(GetDefFunc(programm));
		project.PushBack(*tree);
	}
	return project;
}


node_t& GetDefFunc(List<node_t>& programm)
{
	CheckValidFunc(programm);
	node_t func = programm.ShowFront();
	programm.PopFront();
	CheckOpRoundBr(programm);
	
	node_t* left = GetParamSequence(programm);
	CheckClsRoundBr(programm);
	CheckOpShapeBr(programm);
	node_t* right = GetOpSequence(programm);
	CheckClsShapeBr(programm);
	node_t* function = new node_t(NodeType::WORD, DataType::FUNC, func.value().string_ptr, left, right);
	return *new node_t(NodeType::WORD, DataType::DEFINE, "define", nullptr, function);
}

void CheckValidFunc(List<node_t>& programm)
{
	if (programm.ShowFront().Type() != NodeType::WORD || strcmp(programm.ShowFront().value().string_ptr, "define") != 0)
	{
		std::cout << programm.ShowFront() << "\n";
		fprintf(stderr, "Functions not found\n");
		exit(EXIT_FAILURE);
	}
	programm.PopFront();
	CheckWord(programm);
}


node_t* GetOpSequence(List<node_t>& programm)
{    
	node_t* first_op = GetOperator(programm);
	node_t* current = first_op;
    while (programm.ShowFront().dType() == DataType::END_OP)
    {
		programm.PopFront();
		node_t* second_op = GetOperator(programm);
        current->SetRight(second_op);
		current = second_op;
    }
    return first_op;
}

node_t* GetOperator(List<node_t>& programm)
{
	if (programm.ShowFront().Type() == NodeType::WORD || programm.ShowFront().Type() == NodeType::WORD_WITH_NUMBERS)
	{
		if (strcmp(programm.ShowFront().value().string_ptr, "while") == 0)
		{
			node_t* op = GetWhile(programm);
			op->SetDtype(DataType::WHILE);
			return new node_t(NodeType::OPERATOR, DataType::END_OP, ";", op, nullptr);
		}	
		else if (strcmp(programm.ShowFront().value().string_ptr, "if") == 0)
		{
			node_t* op = GetIf(programm);
			op->SetDtype(DataType::IF);
			return new node_t(NodeType::OPERATOR, DataType::END_OP, ";", op, nullptr);
		}	
		else if (strcmp(programm.ShowFront().value().string_ptr, "return") == 0)
		{
			return new node_t(NodeType::OPERATOR, DataType::END_OP, ";", GetReturn(programm), nullptr);
		}
		else if (strcmp(programm.ShowFront().value().string_ptr, "var") == 0)
		{
			return new node_t(NodeType::OPERATOR, DataType::END_OP, ";", GetInit(programm), nullptr);
		}
		else 
		{
			return new node_t(NodeType::OPERATOR, DataType::END_OP, ";", GetCallFunc(programm), nullptr);
		}
	}
	return nullptr;
}


node_t* GetReturn(List<node_t>& programm)
{
	programm.PopFront();
	return new node_t(NodeType::WORD, DataType::RET, "return", nullptr, GetExpression(programm));
}


node_t* GetRetFunc(List<node_t>& programm)
{
	node_t func = programm.ShowFront();
	programm.PopFront();
	if (programm.ShowFront().dType() == DataType::OP_ROUND_BR)
	{
		return GetFunc(programm, func);
	}
	else 
	{
		programm.PushFront(func);
		return GetVar(programm);
	}
}


node_t* GetCallFunc(List<node_t>& programm)
{
	node_t func = programm.ShowFront();
	programm.PopFront();
	if (programm.ShowFront().dType() == DataType::OP_ROUND_BR)
	{
		return GetFunc(programm, func);
	}
	else 
	{
		programm.PushFront(func);
		return GetAssign(programm);
	}
}


node_t* GetFunc(List<node_t>& programm, node_t& func)
{
	programm.PopFront();
	bool no_string = true;
	node_t* first_parametr = GetArgumentSequence(programm, no_string);
	CheckClsRoundBr(programm);
	char* funcname = func.value().string_ptr;
	if (strcmp(funcname, "input") == 0)
	{
		return new node_t(NodeType::WORD, DataType::SCANF, funcname, nullptr, first_parametr);
	}
	else if (strcmp(funcname, "print") == 0)
	{
		return new node_t(NodeType::WORD, DataType::PRINTF, funcname, nullptr, first_parametr);
	}
	else if (strcmp(funcname, "log") == 0 && no_string)
	{
		return new node_t(NodeType::WORD, DataType::LOG, funcname, nullptr, first_parametr);
	}
	else if (strcmp(funcname, "sin") == 0 && no_string)
	{
		return new node_t(NodeType::WORD, DataType::SIN, funcname, nullptr, first_parametr);
	}
	else if (strcmp(funcname, "cos") == 0 && no_string)
	{
		return new node_t(NodeType::WORD, DataType::COS, funcname, nullptr, first_parametr);
	}
	else if (strcmp(funcname, "sqrt") == 0 && no_string)
	{
		return new node_t(NodeType::WORD, DataType::SQRT, funcname, nullptr, first_parametr);
	}
	else if (no_string)
	{
		return new node_t(NodeType::WORD, DataType::FUNC, funcname, nullptr, first_parametr);
	}
	else
	{
		fprintf(stderr, "Error: bad argument string in function %s\n", funcname);
		exit(EXIT_FAILURE);
	}
}

node_t* GetParamSequence(List<node_t>& programm)
{
	node_t* first_parametr = GetNewVar(programm);
	while (programm.ShowFront().dType() == DataType::COMMA)
	{
		programm.PopFront();
		node_t* second_parametr = GetNewVar(programm);
		first_parametr = new node_t(NodeType::OPERATOR, DataType::COMMA, ",", first_parametr, second_parametr);
	}
	return first_parametr;
}


node_t* GetArgumentSequence(List<node_t>& programm, bool& no_string)
{
	auto GetArgument = GetExpression;
	if (programm.ShowFront().dType() == DataType::QUOTE)
	{
		no_string = false;
		programm.PopFront();
		GetArgument = GetStr;
	}
	
	node_t* first_parametr = GetArgument(programm);
	while (programm.ShowFront().dType() == DataType::COMMA)
	{
		programm.PopFront();
		if (programm.ShowFront().dType() == DataType::QUOTE)
		{
			programm.PopFront();
			GetArgument = GetStr;
		}
		else
		{
			GetArgument = GetExpression;
		}
		node_t* second_parametr = GetArgument(programm);
		first_parametr = new node_t(NodeType::OPERATOR, DataType::COMMA, ",", first_parametr, second_parametr);
	}
	return first_parametr;
}


node_t* GetStr(List<node_t>& programm)
{
	std::string str = programm.ShowFront().Name();
	/*node_t* str = new node_t(programm.ShowFront());
	str->SetDtype(DataType::CONST_STR);*/
	programm.PopFront();
	
	while (programm.ShowFront().dType() != DataType::QUOTE)
	{
		str += " ";
		
		node_t next = programm.ShowFront();
		if (next.Type() == NodeType::NUMBER)
		{
			std::string num = std::to_string(next.value().number);
			str += num;
		}
		else
		{
			str += next.Name();
		}

		programm.PopFront();
	}
	programm.PopFront();
	return new node_t(NodeType::WORD, DataType::CONST_STR, str.c_str());
}



node_t* GetNewVar(List<node_t>& programm)
{
	if ((programm.ShowFront().Type() == NodeType::WORD) && (strcmp(programm.ShowFront().value().string_ptr, "new") == 0))
	{
		programm.PopFront();
		return new node_t(NodeType::WORD, DataType::NEW_VAR, "new", nullptr, GetVar(programm));
	}
	else return GetVar(programm);
}


char* Funcname(node_t& func)
{
	char* funcname = new char[strlen(func.value().string_ptr) + 3];
	strcpy(funcname, func.value().string_ptr);
	strcat(funcname, "()");
	return funcname;
}

node_t* GetInit(List<node_t>& programm)
{
	programm.PopFront();
	CheckWord(programm);

	node_t* lValue = GetVar(programm);
	if (programm.ShowFront().dType() == DataType::MOV)
	{
		programm.PopFront();
		node_t* rValue = GetExpression(programm);
		node_t* expression =  new node_t(NodeType::OPERATOR, DataType::MOV, "=", lValue, rValue);
		return new node_t(NodeType::WORD, DataType::INITIALIZATE, "var", nullptr, expression);
	}
	else
	{
		return new node_t(NodeType::WORD, DataType::INITIALIZATE, "var", nullptr, lValue);
	}

}

void CheckWord(List<node_t>& programm)
{
	if (programm.ShowFront().Type() != NodeType::WORD && programm.ShowFront().Type() != NodeType::WORD_WITH_NUMBERS)
	{
		fprintf(stderr, "The variable or function name must be a word or a word with numbers\n");
		exit(EXIT_FAILURE);
	}
}


node_t* GetAssign(List<node_t>& programm)
{
	node_t* lValue = GetVar(programm);
	if (programm.ShowFront().dType() == DataType::MOV)
	{
		programm.PopFront();
		node_t* rValue = GetExpression(programm);
		return new node_t(NodeType::OPERATOR, DataType::MOV, "=", lValue, rValue);
	}
	else
	{
		fprintf(stderr, "Unknown operator: %s\n", programm.ShowFront().value().string_ptr);
		exit(EXIT_FAILURE);
	}
}


void CheckOpRoundBr(List<node_t>& programm)
{
	if (programm.ShowFront().dType() != DataType::OP_ROUND_BR)
	{
		fprintf(stderr, "Missing opening bracket '('\n");
		exit(EXIT_FAILURE);
	}
	else programm.PopFront();
}

void CheckClsRoundBr(List<node_t>& programm)
{
	if (programm.ShowFront().dType() != DataType::CLS_ROUND_BR)
	{
		fprintf(stderr, "Missing closing bracket ')'\n");
		exit(EXIT_FAILURE);
	}
	else programm.PopFront();
}

void CheckOpShapeBr(List<node_t>& programm)
{
	if (programm.ShowFront().dType() != DataType::OP_SHAPE_BR)
	{
		fprintf(stderr, "Missing opening bracket '{'\n");
		exit(EXIT_FAILURE);
	}
	else programm.PopFront();
}

void CheckClsShapeBr(List<node_t>& programm)
{
	if (programm.ShowFront().dType() != DataType::CLS_SHAPE_BR)
	{
		fprintf(stderr, "Missing closing bracket '}'\n");
		exit(EXIT_FAILURE);
	}
	else programm.PopFront();
}



node_t* GetIf(List<node_t>& programm)
{
	programm.PopFront();

	CheckOpRoundBr(programm);
	node_t* conditions = GetCondExpression(programm);
	CheckClsRoundBr(programm);

	node_t* op_else = GetElse(programm);
	return new node_t(NodeType::OPERATOR, conditions, op_else);
}


node_t* GetElse(List<node_t>& programm)
{
	CheckOpShapeBr(programm);
	node_t* if_op_sequense = GetOpSequence(programm);
	CheckClsShapeBr(programm);

	if (strcmp(programm.ShowFront().value().string_ptr, "else") ==0 )
	{
		programm.PopFront();
		node_t* else_op_sequense = nullptr;
		if (strcmp(programm.ShowFront().value().string_ptr, "if") !=0 )
		{
			CheckOpShapeBr(programm);
			else_op_sequense = GetOpSequence(programm);
			CheckClsShapeBr(programm);
		}
		else
		{
			else_op_sequense = GetIf(programm);
			else_op_sequense->SetDtype(DataType::IF);
		}
		return new node_t(NodeType::WORD, DataType::ELSE, "else", if_op_sequense, else_op_sequense);
	}

	return if_op_sequense;
}




node_t* GetWhile(List<node_t>& programm)
{
	programm.PopFront();

	CheckOpRoundBr(programm);
	node_t* conditions = GetCondExpression(programm);
	CheckClsRoundBr(programm);

	CheckOpShapeBr(programm);
	node_t* oper = GetOpSequence(programm);
	CheckClsShapeBr(programm);
	return new node_t(NodeType::OPERATOR, conditions, oper);
}


node_t* GetCondExpression(List<node_t>& programm)
{
	node_t* cond = GetCondTerm(programm);
	while (programm.ShowFront().dType() == DataType::OR)
	{
		programm.PopFront();
		node_t* cond2 = GetCondTerm(programm);
		cond = new node_t(NodeType::OPERATOR, DataType::OR, "||", cond, cond2);
	}
	return cond;
}


node_t* GetCondTerm(List<node_t>& programm)
{
	node_t* cond = GetPrimaryCondExpression(programm);
	while (programm.ShowFront().dType() == DataType::AND)
	{
		programm.PopFront();
		node_t* cond2 = GetPrimaryCondExpression(programm);
		cond = new node_t(NodeType::OPERATOR, DataType::AND, "&&", cond, cond2);
	}
	return cond;	
}


node_t* GetPrimaryCondExpression(List<node_t>& programm)
{
	if (programm.ShowFront().dType() == DataType::OP_ROUND_BR)
	{
		programm.PopFront();
		node_t* val = GetCondExpression(programm);
		CheckClsRoundBr(programm);
		return val;
	}
	else return GetConditions(programm);	
}


node_t* GetConditions(List<node_t>& programm)
{
	node_t* left_op = GetExpression(programm);
	
	node_t op = programm.ShowFront();
	programm.PopFront();

	node_t* right_op = GetExpression(programm);

	node_t* expression = nullptr;
	switch (op.dType())
	{
	case DataType::JE:
		expression = new node_t(NodeType::OPERATOR, DataType::JE, "==", left_op, right_op);
		break;
	case DataType::JA:
		expression = new node_t(NodeType::OPERATOR, DataType::JA, ">", left_op, right_op);
		break;
	case DataType::JB:
		expression = new node_t(NodeType::OPERATOR, DataType::JB, "<", left_op, right_op);
		break;
	case DataType::JBE:
		expression = new node_t(NodeType::OPERATOR, DataType::JBE, "<=", left_op, right_op);
		break;
	case DataType::JAE:
		expression = new node_t(NodeType::OPERATOR, DataType::JAE, ">=", left_op, right_op);
		break;
	case DataType::JNE:
		expression = new node_t(NodeType::OPERATOR, DataType::JNE, "!=", left_op, right_op);
		break;
	}
	return expression;
}


node_t* GetTerm(List<node_t>& programm)
{
	node_t* val = GetPower(programm);

	while (programm.ShowFront().dType() == DataType::MUL || programm.ShowFront().dType() == DataType::DIV)
	{
		node_t op = programm.ShowFront();
		programm.PopFront();
		node_t* val2 = GetPower(programm);
		if (op.dType() == DataType::MUL)
		{
			val = new node_t(NodeType::OPERATOR, DataType::MUL, "*", val, val2);
		}
		else
		{
			val = new node_t(NodeType::OPERATOR, DataType::DIV, "/", val, val2);
		}
	}
	return val;
}


node_t* GetPower(List<node_t>& programm)
{
	node_t* val = GetPrimaryExpression(programm);
	if (programm.ShowFront().dType() == DataType::DEG)
	{
        programm.PopFront();
		node_t* val2 = GetPower(programm);
		val = new node_t(NodeType::OPERATOR, DataType::DEG, "^", val, val2);
	}
	return val;
}

node_t* GetExpression(List<node_t>& programm)
{
	node_t* val = GetTerm(programm);

	while (programm.ShowFront().dType() == DataType::ADD || programm.ShowFront().dType() == DataType::SUB)
	{
		node_t op = programm.ShowFront();
		programm.PopFront();
		node_t* val2 = GetTerm(programm);
		if (op.dType() == DataType::ADD)
		{
			val = new node_t(NodeType::OPERATOR, DataType::ADD, "+", val, val2);
		}
		else
		{
			val = new node_t(NodeType::OPERATOR, DataType::SUB, "-", val, val2);
		}
	}
	return val;
}


node_t* GetPrimaryExpression(List<node_t>& programm)
{
	if (programm.ShowFront().dType() == DataType::OP_ROUND_BR)
	{
		programm.PopFront();
		node_t* val = GetExpression(programm);
		CheckClsRoundBr(programm);
		return val;
	}
	else return GetRetFunc(programm);
}

node_t* GetVar(List<node_t>& programm)
{
	node_t var = programm.ShowFront();
	if (programm.ShowFront().Type() == NodeType::WORD || programm.ShowFront().Type() == NodeType::WORD_WITH_NUMBERS)
    {
		node_t var = programm.ShowFront();
        programm.PopFront();
        node_t* var_ptr = new node_t(var);
		var_ptr->SetDtype(DataType::VARIABLE);
		return var_ptr;
    }
    else
	{ 
		return GetNumber(programm);
	}

}


node_t* GetNumber(List<node_t>& programm)
{
    if (programm.ShowFront().Type() == NodeType::NUMBER)
    {
		node_t number = programm.ShowFront();
        programm.PopFront();
        return new node_t(number);
    }
	return nullptr;
}
