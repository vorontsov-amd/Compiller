#include "ByteArray.h"


ByteArray::ByteArray()
{
    array = (char*)calloc(1024, 1);
    size = 1024;
    cur_index = 0;
}


ByteArray::ByteArray(const char* _array, const size_t _size)
{
    array = (char*)calloc(_size, 1);
    memcpy(array, _array, _size);
    size = _size;
    cur_index = 0;
}


ByteArray::~ByteArray()
{
    if (array) free(array);
    array = nullptr;
}


index_t ByteArray::Append(const char ch)
{
    if (cur_index == size - 1)
    {
        array = (char*)realloc(array, 2 * size);
        size = 2 * size;
    }
    array[cur_index++] = ch;
    return cur_index;
}

const char* ByteArray::ByteCode() const
{
    return array;
}

size_t ByteArray::Size() const
{
    return size;
}
