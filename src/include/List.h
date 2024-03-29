#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <list>

#define LOX printf("%d\n", __LINE__);

#define ROUND 0
#define LINEAR 1
#define DUMP LINEAR




template <typename T> class List
{
	friend class Iterator;
public:
	explicit List(size_t _capacity = 10);
	List(const List<T>& lst);
	~List();
	
	List<T>& operator=(const List<T>& lst);

	// I wrote this project when I didn't know what &&
	// List<T>& operator=(List<T>&& lst) = delete; 
	// List(List<T>&& lst) = delete;

	void Dump();
	void GraphDump(const char * const graphname = "graph");
	
	int PushBack(const T& value);
	int PushFront(const T& value);
	int Insert(int index, const T& value);
	int Delete(int index);
	T Show(int index);
	T ShowBack();
	T ShowFront();

	int PopFront();
	int PopBack();

	void Sorting();
	bool ListIsSorted();
	
	int TranslateIndex(int logic_index);

	size_t Size() const
	{
		return size;
	}

	class Iterator {
		int index;
		List* list;
	
	public:
		Iterator(int idx, List* lst): index{idx}, list{lst} {} 

		Iterator operator++() {
			index = list->Next[index];
			return *this;
		}

		T operator*() {
			return list->Data[index];
		}

		bool operator!=(Iterator rhs) {
			return index != rhs.index;
		} 

		T* operator->() {
			return &list->Data[index];
		}
	};

	Iterator begin() {
		return {Next[0], this}; 
	}

	Iterator end() {
		return {0, this};
	}

private:
	int* Prev;
	T*   Data;
	int* Next;
	int  free;
	bool list_is_sorted;
	size_t size;
	size_t capacity;
	void ListResize();
	int CheckValidInsertIndex(int index);
	int CheckValidDeleteIndex(int index);
	void InitListArrays(int first_index);
};

template <typename T>			 List<T>::List					(size_t _capacity)
{
	Prev = new int[_capacity] {};
	Data = new T[_capacity] {};
	Next = new int[_capacity] {};

	assert(Prev);
	assert(Data);
	assert(Next);

	free = 1;
	size = 0;
	capacity = _capacity;
	list_is_sorted = true;

	InitListArrays(1);
}

template <typename T>			List<T>::List					(const List<T>& lst)
{
	free = lst.free;
	list_is_sorted = lst.list_is_sorted;
	size = lst.size;
	capacity = lst.capacity;

	Prev = new int[capacity];
	Data = new T[capacity];
	Next = new int [capacity];

	assert(Prev);
	assert(Data);
	assert(Next);

	for (int i = 0; i < capacity; i++)
	{
		Prev[i] = lst.Prev[i];
		Data[i] = lst.Data[i];
		Next[i] = lst.Next[i];
	}
}

template <typename T> List<T>&	 List<T>::operator=			(const List<T>& lst)
{
	while (capacity < lst.capacity)
		ListResize();

	InitListArrays(lst.capacity);

	free = lst.free;
	list_is_sorted = lst.list_is_sorted;
	size = lst.size;


	for (int i = 0; i < lst.capacity; i++)
	{
		Prev[i] = lst.Prev[i];
		Data[i] = lst.Data[i];
		Next[i] = lst.Next[i];
	}

	//for (int i = lst.capacity; i < capacity; i++)
	//{
	//	Data[i] = 0;
	//}

	return *this;
}





template <typename T>			 List<T>::~List				()
{
	delete[] Prev;
	delete[] Data;
	delete[] Next;
}

template <typename T> void		 List<T>::Dump					()
{
	using std::cout;
	using std::endl;
	
	cout << "free: " << free << endl;
	cout << "size: " << size << endl;
	cout << "capacity: " << capacity << endl;

	cout << "Prev:\n";
	for (int i = 0; i < capacity; i++) cout << Prev[i] << " ";
	cout << endl;
	cout << "Data:\n";
	for (int i = 0; i < capacity; i++) cout << Data[i] << " ";
	cout << endl;
	cout << "Next:\n";
	for (int i = 0; i < capacity; i++) cout << Next[i] << " ";
	cout << "\n\n";
}

template <typename T> int		 List<T>::Insert				(int index, const T& value)
{		
	bool status = CheckValidInsertIndex(index);
	if (!status) throw std::invalid_argument("invalid index");
	
	if (size == capacity - 2 or index >= capacity - 1)
		ListResize();

	int RealIndex = free;
	int next_free = -Next[free];
	
	if (size == 0)
	{
		Data[free] = value;
		Next[free] = 0;
		Prev[free] = 0;
		Next[0] = free;
		Prev[0] = free;
	}
	else
	{
		Data[free] = value;
		Prev[free] = index;
		Next[free] = Next[index];
		Prev[Next[index]] = free;
		Next[index] = free;
	}

	free = next_free;
	size += 1;


	if (!ListIsSorted() and index != Prev[0])
		list_is_sorted = false;
	return RealIndex;
}

template <typename T> int		 List<T>::PushBack				(const T& value)
{
	return Insert(Prev[0], value);
}

template <typename T> int		 List<T>::PushFront			(const T& value)
{
	return Insert(0, value);
}

template <typename T> int		 List<T>::Delete				(int index)
{
	bool status = CheckValidDeleteIndex(index);
	if (!status) throw std::invalid_argument("invalid index");
	
	int prev = Prev[index];

	T data = Data[index];
	Next[Prev[index]] = Next[index];
	Prev[Next[index]] = Prev[index];
	Next[index] = -free;
	Prev[index] = -1;
	free = index;
	//Data[index] = 0;
	size--;

	if ((!ListIsSorted() and index != Prev[0]) or (!ListIsSorted() and index != Next[0]))
		list_is_sorted = false;

	return prev;
}

template <typename T> int		 List<T>::PopBack				()
{
	return Delete(Prev[0]);
}

template <typename T> int		 List<T>::PopFront				()
{
	return Delete(Next[0]);
}

template <typename T> T			 List<T>::Show					(int index)
{
	return Data[index];
}

template <typename T> T			 List<T>::ShowBack					()
{
	return Data[Prev[0]];
}

template <typename T> T			 List<T>::ShowFront					()
{
	return Data[Next[0]];
}

template <typename T> void		 List<T>::GraphDump			(const char * const graphname)
{				
	system("mkdir ListDump");

	char* name = (char*)calloc(strlen(graphname) + 10, sizeof(char));
	strcpy(name, "ListDump/");
	strcat(name, graphname);

	size_t length = strlen(name) + 40;
	char* command = (char*)calloc(length, sizeof(char));
	strcpy(command, name);
	strcat(command, ".dot");
	
	std::ofstream dumpfile;
	dumpfile.open(command);

	dumpfile << "digraph G{\n";
	dumpfile << "rankdir=LR\n";
	dumpfile << "edge[color=white]\n";

	size_t index = 0;
	while (index != capacity)
	{
		dumpfile << "Node" << index << "[shape=record, label =\" <index" << index << "> index " << index;
		dumpfile << " | { Prev " << Prev[index];
		dumpfile << "  | "   << Data[index];
		dumpfile << " | <next"   << index << "> Next " << Next[index] << " } \" ];\n";
#if DUMP == LINEAR
		if (index + 1 != capacity)
			dumpfile << "Node" << index << "->Node" << index + 1 << ";\n";
#endif

		index++;
	}

	dumpfile << "edge[color=black]\n";

	index = Next[0];
	while (Next[index] != 0)
	{
		dumpfile << "Node" << index << ":<next" << index << "> -> Node" << Next[index] << ":<index" << Next[index] << ">;\n";
		index = Next[index];
	}
#if DUMP == ROUND
	dumpfile << "Node" << index << ":<next" << index << ">->Node0:<index0>;\n";
	dumpfile << "Node0:<next0> -> Node" << Next[0] << ":<index" << Next[0] << ">; \n";
#endif
	dumpfile << "\"head: " << Next[0] << "\" -> Node" << Next[0] << ":<index" << Next[0] << ">\n";
	dumpfile << "Node" << Prev[0] << ":<index" << Prev[0] << "> -> \"tail: " << Prev[0] << "\"\n";
	dumpfile << "\"free: " << free << "\" -> Node" << free << ":<index" << free << ">\n";
	dumpfile << "\"size: " << size << "\"";
	dumpfile << "\"capacity: " << capacity << "\"";


	dumpfile << "}";
	dumpfile.close();

	strcpy(command, "dot -Tpdf ");
	strcat(command, name);
	strcat(command, ".dot");
	strcat(command, " -o ");
	strcat(command, name);
	strcat(command, ".pdf");
	system(command);
}

template <typename T> void		 List<T>::Sorting				()
{
	T* SortList = new T [capacity]{};
	int SortIndex = 1, DataIndex = Next[0];
	for (SortIndex, DataIndex; DataIndex != Next[Prev[0]]; SortIndex++, DataIndex = Next[DataIndex])
	{
		SortList[SortIndex] = Data[DataIndex];
		Next[SortIndex] = SortIndex + 1;
		Prev[SortIndex] = SortIndex - 1;
	}

	InitListArrays(SortIndex);

	delete[] Data;
	Data = SortList;

	list_is_sorted = true;
}

template <typename T> int		 List<T>::TranslateIndex		(int logic_index)
{		
	int real_index = Next[0];
	for (int i = 1; i < logic_index; i++)
		real_index = Next[real_index];
	return real_index;
}

template <typename T> void		 List<T>::ListResize			()
{
	int* NewPrev = new int[2 * capacity];
	T*   NewData = new T  [2 * capacity];
	int* NewNext = new int[2 * capacity];

	for (int i = 0; i < capacity; i++)
	{
		NewPrev[i] = Prev[i];
		NewData[i] = Data[i];
		NewNext[i] = Next[i];
	}


	delete[] Prev;
	delete[] Data;
	delete[] Next;
		
	Prev = NewPrev;
	Data = NewData;
	Next = NewNext;

	Prev[capacity] = -1;
	Next[capacity] = -int(capacity) - 1;
	for (size_t i = capacity + 1; i < 2 * capacity; i++)
	{
		Prev[i] = -1;
		Next[i] = Next[i - 1] - 1;
	}
	capacity = 2 * capacity;

}

template <typename T> int List<T>::CheckValidDeleteIndex(int index)
{
	return index != 0 and Prev[index] != -1;
}	

template <typename T> int List<T>::CheckValidInsertIndex(int index)
{
	return Prev[index] != -1;
}

template <typename T> void		 List<T>::InitListArrays		(int first_index)
{
	Next[0] = 1;
	Prev[0] = first_index - 1;
	Next[Prev[0]] = 0;
	free = first_index;
	for (int i = first_index, k = 1; i < capacity; i++, k++)
	{
		Prev[i] = -1;
		Next[i] = -free - k;
	}
}

template<typename T> bool List<T>::ListIsSorted			()
{
	return list_is_sorted;
}

