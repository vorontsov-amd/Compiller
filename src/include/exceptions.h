#include <exception>
#include <iostream>
#include <string>

class bad_call : public std::exception
{
private:
    std::string func_name;
public:
    std::string what() 
    { 
        return std::string("exception: bad_call\n") + std::string("function ") + func_name + std::string(" not found\n");
    }
    bad_call(const char* fn) : func_name(fn) { }
    ~bad_call() {}
};
