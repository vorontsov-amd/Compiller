#include "Reading.h"


node_t& GetGrammar(List<node_t>& programm)
{    
	node_t* root = GetOpSequence(programm);
    node_t term = programm.ShowFront();
    programm.PopFront();
    if (term.Type() != NodeType::TERMINATED)
    {
		fprintf(stderr, "THE TERMINATING CHARACTER WAS NOT RECEIVED ");
        std::cout << "RECIVED" << programm.ShowFront();
    }

	return *root;
}


node_t* GetOpSequence(List<node_t>& programm)
{    
	node_t* first_op = GetOperator(programm);
	//std::cout << "first op " << *first_op << "\n";

    while (programm.ShowFront().Type() == NodeType::END_OP)
    {
		programm.PopFront();
			//puts("get op sec 3");

		if (programm.ShowFront().Type() != NodeType::TERMINATED && programm.ShowFront().Type() != NodeType::BRACKET)
		{
			node_t* second_op = GetOperator(programm);
			//std::cout << "second op " << *first_op << "\n";

		    //puts("get op sec 4");

        	first_op = new node_t(NodeType::OPERATOR, DataType::END_OP, ";", first_op, second_op);
		}
		else
		{
			//puts("get op sec 5");
			first_op = new node_t(NodeType::OPERATOR, DataType::END_OP, ";", first_op, nullptr);
		}
    }
    return first_op;
}

node_t* GetOperator(List<node_t>& programm)
{
    // if (programm.ShowFront().Type() != NodeType::WORD)
    // {
    //     fprintf(stderr, "EXPECTED OPERATOR. RECIVED: ");
    //     std::cout << programm.ShowFront();
    // }
	if (programm.ShowFront().Type() == NodeType::WORD)
	{
		if(strcmp(programm.ShowFront().value().string_ptr, "while") == 0)
		{
			programm.ShowFront().SetDtype(DataType::WHILE);
			//std::cout << "сюда дощёл это " << programm.ShowFront() << "\n";
			node_t* op = GetIfWhile(programm);
			op->SetDtype(DataType::WHILE);
			return op;
		}	
		else if(strcmp(programm.ShowFront().value().string_ptr, "if") == 0)
		{
			programm.ShowFront().SetDtype(DataType::WHILE);
			//std::cout << "сюда дощёл это " << programm.ShowFront() << "\n";
			node_t* op = GetIfWhile(programm);
			op->SetDtype(DataType::IF);
			return op;
		}	
		else 
		{
			node_t* lValue = GetVar(programm);
			//std::cout << "lvalue " << *lValue << "\n";

			if (programm.ShowFront().dType() == DataType::MOV)
			{
				programm.PopFront();

				node_t* rValue = GetExpression(programm);
				//std::cout << "rvalue " << *rValue << "\n"; 

				return new node_t(NodeType::OPERATOR, DataType::MOV, "=", lValue, rValue);
			}
			else
			{
				//std::cout << programm.ShowFront();
			}
		}
	}
}


node_t* GetIfWhile(List<node_t>& programm)
{
	programm.PopFront();
	if (programm.ShowFront().dType() != DataType::OP_ROUND_BR)
	{
		//puts("пиздец скобка отъехала");
		//std::cout << "это " << programm.ShowFront();
	}
	else
	{
		programm.PopFront();
		node_t* left_op = GetExpression(programm);
		//std::cout << "left" << *left_op << "\n";
		node_t op = programm.ShowFront();
		programm.PopFront();
		node_t* right_op = GetExpression(programm);
		//std::cout << "right" << *right_op << "\n";
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
		case DataType::JAE:
			expression = new node_t(NodeType::OPERATOR, DataType::JBE, "<=", left_op, right_op);
			break;
		case DataType::JBE:
			expression = new node_t(NodeType::OPERATOR, DataType::JAE, ">=", left_op, right_op);
			break;
		case DataType::JNE:
			expression = new node_t(NodeType::OPERATOR, DataType::JNE, "!=", left_op, right_op);
			break;
		}
		//std::cout << "expr" << *expression << "\n";
		if (programm.ShowFront().dType() != DataType::CLS_ROUND_BR)
		{
			puts("нету скобки бляяяя");
		}
		else 
		{
			programm.PopFront();
			if (programm.ShowFront().dType() == DataType::OP_SHAPE_BR)
			{
				programm.PopFront();
				//std::cout << "я здесь " << programm.ShowFront() << "\n";
				node_t* oper = GetOpSequence(programm);
				if (programm.ShowFront().dType() == DataType::CLS_SHAPE_BR)
				{
					programm.PopFront();
				}
				return new node_t(NodeType::OPERATOR, expression, oper);
			} 
			else 
			{
				//std::cout << "wtf " << programm.ShowFront() << "\n";
			}
		}
		
	}
}


node_t* GetT(List<node_t>& programm)
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
	//puts("get expr 1");
	node_t* val = GetT(programm);
	//puts("get expr 2");

	while (programm.ShowFront().dType() == DataType::ADD || programm.ShowFront().dType() == DataType::SUB)
	{
		node_t op = programm.ShowFront();
		programm.PopFront();
	//	puts("get expr 3");
		node_t* val2 = GetT(programm);
	//	puts("get expr 4");
		if (op.dType() == DataType::ADD)
		{
			val = new node_t(NodeType::OPERATOR, DataType::ADD, "+", val, val2);
		}
		else
		{
			if (val->value().number == 0 && val->Type() == NodeType::NUMBER)
			{
				val = new node_t(NodeType::OPERATOR, DataType::SUB, "-", nullptr, val2);
			}
			else
			{
				val = new node_t(NodeType::OPERATOR, DataType::SUB, "-", val, val2);
			}
		}
		//puts("get expr 5");
	}
	return val;
}


node_t* GetPrimaryExpression(List<node_t>& programm)
{
	if (programm.ShowFront().dType() == DataType::OP_ROUND_BR)
	{
		programm.PopFront();
		node_t* val = GetExpression(programm);
		if (programm.ShowFront().dType() == DataType::CLS_ROUND_BR) puts("SYNTAX ERROR");
		programm.PopFront();
		return val;
	}
	else return GetVar(programm);
}

// node_t* GetF()
// {	
// 	type_t oper = TYPE_unknown;
// 	ReadFunc(&oper);

// 	if (oper != TYPE_unknown)
// 	{
// 		node_t* val = GetP();
// 		data_t func(oper);
// 		return new node_t(func, nullptr, val);
// 	}
// 	else return GetV();
// }


node_t* GetVar(List<node_t>& programm)
{
    //puts("get var 0");
	node_t var = programm.ShowFront();
    //puts("get var 1");
	if (programm.ShowFront().Type() != NodeType::WORD || programm.ShowFront().Type() != NodeType::WORD_WITH_NUMBERS)
    {
		node_t var = programm.ShowFront();
        programm.PopFront();
        return new node_t(var);
    }
    else
	{ 
		//puts("get var 2");  
		return GetNumber(programm);
	}

}


node_t* GetNumber(List<node_t>& programm)
{
    if (programm.ShowFront().Type() == NodeType::NUMBER)
    {
		node_t number = programm.ShowFront();
		//std::cout << number; 
        programm.PopFront();
        return new node_t(number);
    }
}


// int ReadVar(char* c)
// {
// 	if ('a' <= s[p] && s[p] <= 'z')
// 	{
// 		*c = s[p];
// 		p++;
// 	}
// 	return 0;
// }


// int StrEqual(const char* l, const char* r)
// {
// 	if (l == nullptr || r == nullptr) return 0;

// 	for (int i = 0; l[i] != '\0' && r[i] != '\0'; i++)
// 		if (l[i] != r[i]) return 0;

// 	return 1;
// }


// int ReadFunc(type_t* n)
// {
// 	const char* str = &s[p];


//     if (StrEqual(str, "sin"))
//     {
// 		p += 3;
// 		*n = TYPE_sin;
//     }
//     else if (StrEqual(str, "cos"))
// 	{
// 		p += 3;
// 		*n = TYPE_cos;
// 	}
// 	else if (StrEqual(str, "tg"))
// 	{
// 		p += 2;
// 		*n = TYPE_tan;
// 	}
// 	else if (StrEqual(str, "ctg"))
// 	{
// 		p += 3;
// 		*n = TYPE_cot;
// 	}
// 	else if (StrEqual(str, "lg"))
// 	{
// 		p += 2;
// 		*n = TYPE_lg;
// 	}
// 	else if (StrEqual(str, "sh"))
// 	{
// 		p += 2;
// 		*n = TYPE_sinh;
// 	}
// 	else if (StrEqual(str, "ch"))
// 	{
// 		p += 2;
// 		*n = TYPE_cosh;
// 	}
// 	else if (StrEqual(str, "th"))
// 	{
// 		p += 2;
// 		*n = TYPE_tanh;
// 	}
// 	else if (StrEqual(str, "cth"))
// 	{
// 		p += 3;
// 		*n = TYPE_coth;
// 	}
// 	else
// 	{
// 		*n = TYPE_unknown;
// 		return -1;
// 	}
//     return 0;
// }

