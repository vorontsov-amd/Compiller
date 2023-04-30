# My onw programming language
A fully compiled open source language, so anyone can name keywords as they please. In addition to the binary executable file, the compiler generates a graphical dump in the form of a binary program tree and an x86-64 assembler listing in nasm syntax.
## Syntax
Since it is a procedural programming language, program blocks are functions. There is no "main()" function here, so the program will start its execution with the function declared in the file first.
### Key words
* define - announces that the definition of a custom function is about to begin.
* new - specifies that the function should create a local variable initialized by the accepted function argument. In fact, without specifying this keyword, the variable will be passed to the function by reference, and with the keyword "new" by value.
* var - declares a new variable.
* while - declares a while loop.
* if - declares an if branch operator.
* else - declares an else branch operator.
* return - causes premature exit from the function and returns the value behind this operator.
### Operators
* +, - , * , / , ^ - math operators
* ==, >=, <=, <, >, != - compare operators
* &&, || - logic operators
* if, else, - branching operator
* while - loop operator
* return - return operator
* each operator in the body of the function must end with ";".
* operators in the bodies "if", "else" and "while" are separated by "{" and "}". The exception is the "if" operator standing after "else", which allows you to build "else if" constructions.
### Standart functions
* print() - prints the arguments specified in parentheses to the console. It can be a variable, an expression, a numeric or string literal. The function supports any number of arguments. All arguments of the function are printed separated by a space. After that, a newline character is printed. The string literal is separated by single quotes ('something').
* input() - supports two modes: 1) With the return value. All arguments are printed to the console in the same way as the print() function, reads the argument from the console, after which the return value of the function is assigned lvalue. 2) With an unlimited number of variables. All arguments must be variables, after which the function reads from the console the number of numbers equal to the number of arguments and assigns them.
* sin() - return sin argument
* cos() - return cos argument
* log() - return log10 argument
* sqrt() - return sqrt argument

## Installing

In order to install my compiler you need to clone this repository

```
git clone git@github.com:vorontsov-amd/Compiller.git
cd Compiller
```
After that, compile the project using CMake

## Compiling 
If you want to recompile the compiler for my language, you can do it using Makefile.
In order to compile the program in my language, write to the console
```cmd
> ./compl output_file_name input_file_name
```
If you want to compile an assembler listing, then use nasm and link together with the std.o file.

### Example of how the program works
input file:

![alt-text](https://sun9-46.userapi.com/impg/RR8UoVI7hY8tfus6Szk_asvR0q6PicY2xF1JIw/NUEubNJaQ_A.jpg?size=432x215&quality=96&sign=a5df428ece99359faefa2ea50b11c324&type=album)

compilling and launch:

![](https://sun9-51.userapi.com/impg/3NwNOkAVWdoT_ihTNaLrfLzY6JrcvF-gfDyYbg/7NXmFWO_aX4.jpg?size=1285x202&quality=96&sign=52e018d2918d21c74f5dc34f0db51190&type=album)
graph dump:

![](https://sun9-28.userapi.com/impg/TzfzuZe4n7yj_AZbOKK1C0R1zegjVkU7trXblw/Oprr823S5Ws.jpg?size=718x772&quality=96&sign=0a8686572bd8ec146ddbe92e485c59a0&type=album)

asseble listing dump:

![](https://sun9-76.userapi.com/impg/wPtPKIUM6Y2um5gcp6lxVY5x3PTxN5PgFBqpaQ/ljNb5u6jOhA.jpg?size=949x902&quality=96&sign=0b58553eacd308c3e5161ede68120f73&type=album)


