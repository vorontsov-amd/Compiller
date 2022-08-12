#pragma once
#include <iostream>
#include "data_t.h"
#include <cassert>

class variable
{
private:
    char* var;
    bool is_link;
    uint64_t offset;
public:
    variable();
    variable(node_t*, bool, uint64_t);
    ~variable();

    uint64_t Offset() const;
    const char* Name() const;
    bool IsLink() const;
    variable& operator=(const variable& var);
};
