#include "Differenciator.h"
#pragma warning(disable:4996)


DifferTree::DifferTree()
{
	root = nullptr;
}

DifferTree::DifferTree(DifferTree& tree)
{
	root = new node_t(tree.root);
}

DifferTree::DifferTree(DifferTree&& tree)
{
	root = new node_t(tree.root);
}


DifferTree::DifferTree(node_t* _root)
{
	root = new node_t(_root);
}

DifferTree::DifferTree(node_t& _root)
{
	root = new node_t(_root);
}

DifferTree::~DifferTree()
{
	if (root) Free(root);
}

void DifferTree::Write(std::ofstream& file, node_t* node)
{
	file << "(";
	if (node->left)  Write(file, node->left);
	file << node->data;
	if (node->right) Write(file, node->right);
	file << ")"; 
		
}

void DifferTree::Read(std::ifstream& file, node_t*& node, data_t& element)
{
	char test = 0;
	file >> test;
	if (test == '(')
	{
		CreateNode(element, node);
		Read(file, node->left, element);
		file >> element;
		node->data = element;
		Read(file, node->right, element);
	}
	else
	{
		file.putback(test);
		return;
	}
	file >> test;
}

int DifferTree::Scan(const char* filename)
{
	std::ifstream in;
	in.open(filename);
	data_t element;
	DifferTree::Read(in, root, element);
	in.close();

	return 0;
}

int DifferTree::Print()
{
	std::ofstream out;
	out.open("DataBase.txt");
	Write(out, root);
	out.close();

	return 0;
}


void DifferTree::GraphDump(const char* graphname)
{
	std::string name = std::string("TreeDump/") + graphname;
	std::string command = name + ".dot";
	
	std::ofstream dumpfile;
	dumpfile.open(command);

	dumpfile << "digraph " << graphname << "{\n";
	DumpNode(dumpfile, root);
	dumpfile << "}";

	dumpfile.close();

	command = "iconv -f cp1251 -t utf-8 " + name + ".dot > " + name + "-utf8.dot";
	const char* command_c = command.c_str();
	system(command_c);

	command = "dot -Tpdf " + name + "-utf8.dot -o " + name + ".pdf";
	command_c = command.c_str();

	system(command_c);
}

void DifferTree::DumpNode(std::ofstream& dumpfile, const node_t* node)
{
	dumpfile << "Node" << node << "[shape=\"record\", label=\"" << node->data << "\"];\n";

	if (node->left)
	{
		DumpNode(dumpfile, node->left);
		dumpfile << "Node" << node << "->" << "Node" << node->left << ";\n";
	}
	if (node->right)
	{
		DumpNode(dumpfile, node->right);
		dumpfile << "Node" << node << "->" << "Node" << node->right << ";\n";
	}
}

void DifferTree::Free(node_t* tree)
{
	if (tree->left)  Free(tree->left);
	if (tree->right) Free(tree->right);
	if (tree)
	{
		delete tree;
		tree = nullptr;
	}
}

void DifferTree::CreateNode(const data_t& data, node_t*& node)
{
	node_t* new_node = new node_t;
	new_node->data = data;
	node = new_node;
}
//---------------------------------------END-------------------------------------------------


void DifferTree::SwapSon(node_t* parrent)
{
	node_t* node = new node_t(parrent->left);
	parrent->left = parrent->right;
	parrent->right = node;
}


DifferTree& DifferTree::operator=(const DifferTree& tree)
{
	if (root == nullptr)
	{
		root = new node_t;
	}
	if (tree.root)
	{
		*root = *tree.root;
	}
	return *this;
}
