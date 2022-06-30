#include "data_t.h"

#define function(f)				\
	if (strcmp(func, #f) == 0)	 \
		data.type = TYPE_##f;

#define case_func(f)		\
	case TYPE_##f:			 \
		stream << #f;		  \
		break;


//------------------------CLASS-data_t---------------------------------------	
std::istream& operator>> (std::istream& stream, data_t& data)
{
	char operand = 0;
	stream >> operand;

	switch (operand)
	{
	case DataType::ADD: data.type = DataType::ADD; break;
	case DataType::SUB: data.type = DataType::SUB; break;
	case DataType::MUL: data.type = DataType::MUL; break;
	case DataType::DIV: data.type = DataType::DIV; break;
	case DataType::DEG: data.type = DataType::DEG; break;
	default:
		if (isalpha(operand))
		{
			char func[5] = { 0 };
			func[0] = operand;
			operand = stream.get();
			for (int i = 1; isalpha(operand); i++, operand = stream.get())
				func[i] = operand;

			stream.putback(operand);
			if (func[1] == '\0')
			{
				data.type = DataType::VARIABLE;
				data.value.string_ptr = new char[2];
				data.value.string_ptr[0] = func[0];
				data.value.string_ptr[1] = '\0';
			}
		}
		else
		{
			stream.putback(operand);
			double num = 0;
			stream >> num;
			data.type = DataType::CONSTANT;
			data.value.number = num;
		}
		break;
	}
	return stream;
}

std::ostream& operator<< (std::ostream& stream, const data_t& data)
{
	stream << "Data: ";
	switch (data.type)
	{
	case DataType::UNKNOWN:
		stream << data.value.string_ptr;
		break;
	case DataType::CONSTANT:
		stream << data.value.number;
		break;
	case DataType::VARIABLE:
		stream << data.value.string_ptr;
		break;
	default:
		stream << data.value.string_ptr;
		break;
	}
	return stream;
}

data_t::data_t(data_t& data)
{
	type = data.type;
	value = data.value;
}

data_t::data_t(DataType::dataType _type, double _value)
{
	type = _type;
	value.number = _value;
}

data_t::data_t(DataType::dataType _type, const char* str)
{
	type = _type;
	value.string_ptr = new char[strlen(str) + 1];
	strcpy(value.string_ptr, str);
}

data_t::data_t(DataType::dataType _type)
{
	type = _type;
	value.string_ptr = new char[8];
	strcpy(value.string_ptr, "nothing");
}

data_t::data_t()
{
	type = DataType::UNKNOWN;
	value.string_ptr = new char[8];
	strcpy(value.string_ptr, "nothing");
}

data_t::~data_t()
{
}

data_t& data_t::operator=(const data_t& data)
{
	type = data.type;
	value = data.value;
	return *this;
}
//-------------------------------END-----------------------------------------
// 
// 
//------------------------CLASS-node_t---------------------------------------
node_t::node_t()
{
	data = data_t();
	type = NodeType::UNKNOWN;
	left = nullptr;
	right = nullptr;
}

node_t::node_t(const data_t& _data)
{
	data = _data;
	type = NodeType::UNKNOWN;
	left = nullptr;
	right = nullptr;
}

node_t::node_t(const NodeType::nodeType _type)
{
	type = _type;
	data = data_t();
	left = nullptr;
	right = nullptr;
}

node_t::node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, char _data)
{
	type = N_type;
	data.type = D_type;
	data.value.string_ptr = new char[2];
	data.value.string_ptr[0] = _data;
	data.value.string_ptr[1] = '\0';
	left = nullptr;
	right = nullptr;
}

// node_t::node_t(const NodeType::nodeType _type, char _data)
// {
// 	type = _type;
// 	data.value.string_ptr = new char[2];
// 	data.value.string_ptr[0] = _data;
// 	data.value.string_ptr[1] = '\0';
// 	left = nullptr;
// 	right = nullptr;
// }

node_t::node_t(const NodeType::nodeType N_type, const DataType::dataType D_type)
{
	type = N_type;
	data = data_t(D_type);
	left = nullptr;
	right = nullptr;
}


node_t::node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, double number)
{
	type = N_type;
	data = data_t(D_type, number);
	left = nullptr;
	right = nullptr;
}

node_t::node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, const char* str)
{
	type = N_type;
	data = data_t(D_type, str);
	left = nullptr;
	right = nullptr;
}


node_t::node_t(const data_t& _data, node_t* _left, node_t* _right)
{
	data = _data;
	type = NodeType::UNKNOWN;
	left = _left;
	right = _right;
}

node_t::node_t(const NodeType::nodeType _type, node_t* _left, node_t* _right)
{
	type = _type;
	data = data_t();
	left = _left;
	right = _right;
}

node_t::node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, const char* str, node_t* _left, node_t* _right)
{
	type = N_type;
	data = data_t(D_type, str);
	left = _left;
	right = _right;
}

node_t::node_t(const NodeType::nodeType N_type, const DataType::dataType D_type, node_t* _left, node_t* _right)
{
	type = N_type;
	data = data_t(D_type);
	left = _left;
	right = _right;
}

node_t::node_t(node_t& node)
{
	data = node.data;
	type = node.type;
	if (node.left != nullptr)
	{
		left = new node_t(*node.left);
	}
	else 
	{
		left = nullptr;
	}

	if (node.right != nullptr)
	{
		right = new node_t(*node.right);
	}
	else 
	{
		right = nullptr;
	}
}

node_t::node_t(node_t* node)
{
	data = node->data;
	type = node->type;
	if (node->left != nullptr)
	{
		left = new node_t(*node->left);
	}
	else 
	{
		left = nullptr;
	}

	if (node->right != nullptr)
	{
		right = new node_t(*node->right);
	}
	else 
	{
		right = nullptr;
	}
}

node_t::~node_t()
{
}

node_t& node_t::operator=(const node_t& node)
{
	data = node.data;
	type = node.type;
	if (left != nullptr && node.left != nullptr)
	{
		*left = *node.left;
	}
	else if (left == nullptr && node.left != nullptr)
	{
		left = new node_t;
		*left = *node.left;
	}
	else if (left != nullptr && node.left == nullptr)
	{
		delete left;
		left = nullptr;
	}
	if (right != nullptr && node.right != nullptr)
	{
		*right = *node.right;
	}
	else if (right == nullptr && node.right != nullptr)
	{
		right = new node_t;
		*right = *node.right;
	}
	else if (right != nullptr && node.right == nullptr)
	{
		delete right;
		right = nullptr;
	}
	return *this;
}

node_t& operator+(node_t& node_left, node_t& node_right)
{
	data_t new_data(DataType::ADD);
	return *new node_t(new_data, &node_left, &node_right);
}

node_t& operator-(node_t& node_left, node_t& node_right)
{
	data_t new_data(DataType::SUB);
	return *new node_t(new_data, &node_left, &node_right);
}

node_t& operator*(node_t& node_left, node_t& node_right)
{
	data_t new_data(DataType::MUL);
	return *new node_t(new_data, &node_left, &node_right);
}

node_t& operator^(node_t& node_left, node_t& node_right)
{
	data_t new_data(DataType::DEG);
	return *new node_t(new_data, &node_left, &node_right);
}

node_t& operator/(node_t& node_left, node_t& node_right)
{
	data_t new_data(DataType::DIV);
	return *new node_t(new_data, &node_left, &node_right);
}

node_t& operator^(node_t& node_left, double deg)
{
	data_t new_data(DataType::DEG);
	data_t data_degree(DataType::CONSTANT, deg);
	node_t* degree = new node_t(data_degree);
	return *new node_t(new_data, &node_left, degree);
}

node_t& node_t::operator-()
{
	data_t sub(DataType::SUB);
	node_t* right_node = new node_t(this);
	data = sub;
	left = nullptr;
	right = right_node;
	return *this;
}


//-------------------------------END-----------------------------------------


#undef function
#undef case_func

