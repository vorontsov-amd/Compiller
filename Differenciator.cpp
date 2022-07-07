#include "Differenciator.h"
#pragma warning(disable:4996)


DifferTree::DifferTree()
{
	root = nullptr;
	current_node = root;
}

DifferTree::DifferTree(DifferTree& tree)
{
	//std::cout << tree.root << "\n";
	root = new node_t(tree.root);
	current_node = root;
}

DifferTree::DifferTree(DifferTree&& tree)
{
	root = new node_t(tree.root);
	current_node = root;
}


DifferTree::DifferTree(node_t* _root)
{
	root = new node_t(_root);
	current_node = root;
}

DifferTree::DifferTree(node_t& _root)
{
	root = new node_t(_root);
	current_node = root;
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
	char* name = (char*)calloc(strlen(graphname) + 10, sizeof(char));
	strcpy(name, "TreeDump/");
	strcat(name, graphname);
	
	size_t length = strlen(name) + 50;
	char* command = new char[length] {};
	strncpy(command, name, length);
	strncat(command, ".dot", length);

	std::ofstream dumpfile;
	dumpfile.open(command);

	dumpfile << "digraph " << graphname << "{\n";
	DumpNode(dumpfile, root);
	dumpfile << "}";

	dumpfile.close();

	strncpy(command, "iconv -f cp1251 -t utf-8 ", length);
	strncat(command, name, length);
	strncat(command, ".dot > ", length);
	strncat(command, name, length);
	strncat(command, "-utf8.dot", length);
	system(command);

	strncpy(command, "dot -Tpdf ", length);
	strncat(command, name, length);
	strncat(command, "-utf8.dot", length);
	strncat(command, " -o ", length);
	strncat(command, name, length);
	strncat(command, ".pdf", length);

	system(command);
	delete[] command;
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


node_t* DifferTree::ShowCurrent()
{
	return current_node;
}
node_t* DifferTree::UpdateCurrent(answer ans)
{
	if (ans == answer::right)
	{
		if (current_node->right) current_node = current_node->right;
		else return nullptr;
	}
	else if (ans == answer::left)
	{
		if (current_node->left) current_node = current_node->left;
		else return nullptr;
	}
	else if (ans == answer::root)
	{
		current_node = root;
	}
	return current_node;
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
