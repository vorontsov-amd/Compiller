// #include "Differenciator.h"
// using namespace iLab;
// static const char* s = nullptr;
// static int p = 0;

// node_t* GetN()
// {
// 	int val = 0;

// 	int save_p = p;
// 	while ('0' <= s[p] && s[p] <= '9')
// 	{
// 		val = val * 10 + (s[p] - '0');
// 		p++;
// 	}
// 	data_t num(TYPE_int_constant, val);
// 	node_t* val_node = new node_t(num, nullptr, nullptr);
// 	return val_node;
// }

// node_t* GetT()
// {
// 	node_t* val = GetW();

// 	while (s[p] == '*' || s[p] == '/')
// 	{
// 		int op = s[p];
// 		p++;
// 		node_t* val2 = GetW();
// 		if (op == '*')
// 		{
// 			data_t mul(TYPE_mul);
// 			val = new node_t(mul, val, val2);
// 		}
// 		else
// 		{
// 			data_t div(TYPE_div);
// 			val = new node_t(div, val, val2);
// 		}
// 	}
// 	return val;
// }


// node_t* GetW()
// {
// 	node_t* val = GetP();
// 	if (s[p] == '^')
// 	{
// 		p++;
// 		node_t* val2 = GetW();
// 		data_t pow(TYPE_deg);
// 		val = new node_t(pow, val, val2);
// 	}
// 	return val;
// }

// node_t* GetE()
// {
// 	node_t* val = GetT();

// 	while (s[p] == '+'|| s[p] == '-')
// 	{
// 		int op = s[p];
// 		p++;
// 		node_t* val2 = GetT();
// 		if (op == '+')
// 		{
// 			data_t add(TYPE_add);
// 			val = new node_t(add, val, val2);
// 		}
// 		else
// 		{
// 			data_t sub(TYPE_sub);
// 			if (val->value().int_value == 0 && val->type() == TYPE_int_constant)
// 			{
// 				val = new node_t(sub, nullptr, val2);
// 			}
// 			else
// 			{
// 				val = new node_t(sub, val, val2);
// 			}
// 		}
// 	}
// 	return val;
// }


// node_t* GetP()
// {
// 	if (s[p] == '(')
// 	{
// 		p++;
// 		node_t* val = GetE();
// 		if (s[p] != ')') puts("SYNTAX ERROR");
// 		p++;
// 		return val;
// 	}
// 	else return GetF();
// }

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


// node_t* GetV()
// {
// 	char var = 0;
// 	ReadVar(&var);

// 	if (var != 0)
// 	{
// 		data_t func(TYPE_variable, var);
// 		return new node_t(func, nullptr, nullptr);
// 	}
// 	else return GetN();
// }


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


// node_t* GetG(const char* str)
// {
// 	s = str;
// 	p = 0;
// 	node_t* val = GetE();
// 	if (s[p] != '\0')
// 		puts("SYNTAX ERROR");
// 	return val;
// }

// namespace iLab
// { 
// 	int DifferTree::TreeScan(const char* filename)
// 	{
// 		std::ifstream in;
// 		in.open(filename);
		
// 		int filesize = Filesize(in);
// 		char* buffer = new char[filesize + 1];
// 		in >> buffer;
// 		buffer[filesize] = '\0';
// 		root = GetG(buffer);

// 		in.close();

// 		return 0;
// 	}

// 	int Filesize(std::ifstream& f)
// 	{
// 		int count = 0;
// 		char ch = 0;
// 		while (f >> ch)
// 			++count;
// 		f.clear();
// 		f.seekg(0);
// 		return count;
// 	}
// }


