#include "Variable.h"


variable::variable()
{
    var = nullptr;
    is_link = false;
    offset = 0xBAD00BAD;
}


variable::variable(node_t* _var, bool _is_link, uint32_t _offset)
{
    assert(_var);
    const char* name = _var->Name();
    assert(name);
    var = new char[strlen(name) + 1];
    strcpy(var, name);
    is_link = _is_link;
    offset = _offset;
}



variable::~variable()
{
    
}


uint32_t variable::Offset() const
{
    return offset;
}


const char* variable::Name() const
{
    return var;
}


bool variable::IsLink() const
{
    return is_link;
}


variable& variable::operator=(const variable& _var)
{
    const char* name = _var.var;
    if (name) 
    {
        var = new char[strlen(name) + 1];
        strcpy(var, name);
    }
    is_link = _var.is_link;
    offset = _var.offset;
    return *this;
}
