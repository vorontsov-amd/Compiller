#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define LOX printf("%d\n", __LINE__);


class data_t;
class DifferTree;
class node_t;
union value_t;

namespace DataType
{
	enum dataType
	{
		ADD = '+',
		SUB = '-',
		MUL = '*',
		DIV = '/',
		DEG = '^',
		OP_ROUND_BR = '(',
		CLS_ROUND_BR = ')',
		OP_SQUARE_BR = '[',
		CLS_SQUARE_BR = ']',
		OP_SHAPE_BR = '{',
		CLS_SHAPE_BR = '}',
		COMMA = ',',
		MOV = '=',
		JA = '>',
		JB = '<',
		JE = 256,
		JBE,
		JAE,
		JNE,
		WHILE,
		IF,
		ELSE,
		END_OP,
		UNKNOWN,
		INITIALIZATE,
		CONSTANT,
		VARIABLE,
		FUNC,
		SCANF,
		PRINTF,
		SQRT,
		DEFINE,
		NEW_VAR,
		RET,
		QUOTE,
		CONST_STR,
		AND,
		OR,
		SIN,
		COS,
		LOG,
	};
}

namespace NodeType
{
	enum nodeType
	{
		TERMINATED,
		NUMBER,
		WORD,
		OPERATOR,
		BRACKET,
		WORD_WITH_NUMBERS,
		END_OP,
		UNKNOWN,
	};
}

union value_t
{
	double number;
	char* string_ptr;
};

class data_t
{
	friend DifferTree;
	friend node_t;

	friend std::istream& operator>> (std::istream& stream, data_t& data);
	friend std::ostream& operator<< (std::ostream& stream, const data_t& data);
public:
	data_t();
	~data_t();
	data_t(data_t& data);
	data_t(DataType::dataType _type, double _value);
	data_t(DataType::dataType _type, const char* _value);
	data_t(DataType::dataType);

	data_t& operator=(const data_t& data);
	bool operator==(double num)
	{
		return value.number == num;
	}

private:
	DataType::dataType type;
	value_t value;
};

class node_t
{
	friend DifferTree;
	friend std::ostream& operator<<(std::ostream& stream, node_t const& node)
	{
		stream << "Node type is ";
		switch (node.type)
		{
		case NodeType::NUMBER:
			stream << "NUMBER. ";
			break;
		case NodeType::OPERATOR:
			stream << "OPERATOR. ";
			break;
		case NodeType::TERMINATED:
			stream << "TERMINATED. ";
			break;
		case NodeType::UNKNOWN:
			stream << "UNKNOWN. ";
			break;
		case NodeType::WORD:
			stream << "WORD. ";
			break;
		case NodeType::WORD_WITH_NUMBERS:
			stream << "WORD_WITH_NUMBERS. ";
			break;
		case NodeType::BRACKET:
			stream << "BRACKET. ";
			break;
		case NodeType::END_OP:
			stream << "END_OP. ";
			break;
		default:
			break;
		}
		return stream << node.data;
	}
	friend node_t& operator+(node_t& node_left, node_t& node_right);
	friend node_t& operator-(node_t& node_left, node_t& node_right);
	friend node_t& operator*(node_t& node_left, node_t& node_right);
	friend node_t& operator/(node_t& node_left, node_t& node_right);
	friend node_t& operator^(node_t& node_left, double deg);
	friend node_t& operator^(node_t& node_left, node_t& node_right);



public:
	node_t();
	node_t(const data_t& _data);
	node_t(node_t& node);
	node_t(const NodeType::nodeType _type);
	node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, char _value);
	node_t(node_t* node);
	node_t(const data_t& _data, node_t* _left, node_t* _right);
	node_t(const NodeType::nodeType _type, node_t* _left, node_t* _right);
	node_t(const NodeType::nodeType N_type, const DataType::dataType D_type);
	node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, double number);
	node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, const char* str);
	node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, const char* str, node_t* _left, node_t* _right);
	node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, node_t* _left, node_t* _right);



	~node_t();

	node_t& operator=(const node_t& node);
	node_t& operator-(int number);
	node_t& operator-();
	operator data_t()
	{
		return data;
	}

	value_t value()
	{
		return data.value;
	}

	NodeType::nodeType Type() const
	{
		return type;
	}

	NodeType::nodeType& Type()
	{
		return type;
	}

	DataType::dataType dType()
	{
		return data.type;
	}

	void SetDtype(DataType::dataType Dtype)
	{
		data.type = Dtype;
		switch (Dtype)
		{
		case DataType::WHILE:
			data.value.string_ptr = new char[6];
			strcpy(data.value.string_ptr, "while");
			break;
		case DataType::IF:
			data.value.string_ptr = new char[6];
			strcpy(data.value.string_ptr, "if");
			break;
		case DataType::VARIABLE:
			break;
		default:
			puts("Update functions SetDtype()");
			break;
		}
	}
	
	node_t& SetLeft(node_t* _left)
	{
		left = _left;
		return *this;
	}

	node_t& SetRight(node_t* _right)
	{
		right = _right;
		return *this;
	}

	node_t* GetLeft()
	{
		return left;
	}

	node_t* GetRight()
	{
		return right;
	}

	const char* Name()
	{
		return data.value.string_ptr;
	}

protected:
	data_t data;
	NodeType::nodeType type;

	node_t* left;
	node_t* right;
};