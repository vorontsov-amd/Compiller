#include "Variable.h"


variable::variable()
{
    var = nullptr;
    is_link = false;
    offset = 0;
}


variable::variable(node_t* _var, bool _is_link, uint64_t _offset)
{
    var = _var;
    is_link = _is_link;
    offset = _offset;
}



variable::~variable()
{
    
}


uint64_t variable::Offset() const
{
    return offset;
}


const char* variable::Name() const
{
    return var->value().string_ptr;
}


bool variable::IsLink() const
{
    return is_link;
}