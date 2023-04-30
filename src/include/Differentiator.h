#pragma once
#include "data_t.h"

#define FUCK fprintf(stderr, "%d %s\n", __LINE__, __FUNCSIG__);


enum class answer
{
	left,
	right,
	root,
};




class DifferTree;	
node_t& CopyNode(node_t& node);
void Filesize(FILE* stream, size_t* filesize);
void ReadBuffer(char** buffer, FILE* stream);
int Filesize(std::ifstream& f);



class DifferTree
{
	friend void TreeRead(DifferTree& tree, char* str);
public:
	DifferTree();
	DifferTree(DifferTree& tree);
	DifferTree(DifferTree&& tree);
	DifferTree(node_t* _root);
	DifferTree(node_t& _root);
	~DifferTree();
	void Free(node_t* tree);

	DifferTree& Simple();

	int Scan(const char* filename = "formula.txt");
	int TreeScan(const char* filename = "formula.txt");
	int Print();
	void GraphDump(const char* graphname = "Dump");
	DifferTree& operator=(const DifferTree& tree);


	node_t* Root()
	{
		return root;
	}

private:
	node_t* root;

	void CreateNode(const data_t& data, node_t*& node);
	void Read(std::ifstream& file, node_t*& node, data_t& element);
	void Write(std::ofstream& file, node_t* node);
	void DumpNode(std::ofstream& dumpfile, const node_t* node);
	void SwapSon(node_t* parrent);

};