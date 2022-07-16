#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>


typedef unsigned long long index_t;

class ByteArray
{
private:
    char* array;
    size_t size;
    index_t cur_index;
public:
    ByteArray();
    ByteArray(const char* _array, const size_t _size);
    ~ByteArray();

    index_t Append(const char ch);
    const char* ByteCode() const;
    size_t Size() const;
};