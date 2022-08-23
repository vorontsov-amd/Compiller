# My onw programming language
A fully compiled open source language, so anyone can name keywords as they please. In addition to the binary executable file, the compiler generates a graphical dump in the form of a binary program tree and an x86-64 assembler listing in nasm syntax.
### Syntax
Since it is a procedural programming language, program blocks are functions. There is no "main()" function here, so the program will start its execution with the function declared in the file first.
##### Key words
* define - announces that the definition of a custom function is about to begin.
* new - specifies that the function should create a local variable initialized by the accepted function argument. In fact, without specifying this keyword, the variable will be passed to the function by reference, and with the keyword "new" by value.
* var - declares a new variable.
* while - declares a while loop.
* if - declares an if branch operator.
* else - declares an else branch operator.
* return - causes premature exit from the function and returns the value behind this operator.
##### Operators
* +, - , * , / , ^ - math operators
* ==, >=, <=, <, >, != - compare operators
* &&, || - logic operators
* if, else, - branching operator
* while - loop operator
* return - return operator
* each operator in the body of the function must end with ";".
* operators in the bodies "if", "else" and "while" are separated by "{" and "}". The exception is the "if" operator standing after "else", which allows you to build "else if" constructions.
##### Standart functions
* print() - prints the arguments specified in parentheses to the console. It can be a variable, an expression, a numeric or string literal. The function supports any number of arguments. All arguments of the function are printed separated by a space. After that, a newline character is printed. The string literal is separated by single quotes ('something').
* input() - supports two modes: 1) With the return value. All arguments are printed to the console in the same way as the print() function, reads the argument from the console, after which the return value of the function is assigned lvalue. 2) With an unlimited number of variables. All arguments must be variables, after which the function reads from the console the number of numbers equal to the number of arguments and assigns them.
* sin() - return sin argument
* cos() - return cos argument
* log() - return log10 argument
* sqrt() - return sqrt argument
