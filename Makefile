CC=g++

CFLAGS=-c

all: data_t.cpp Differenciator.cpp lexical_anaysis.cpp main.cpp Reading.cpp
	g++ main.cpp data_t.cpp Differenciator.cpp lexical_anaysis.cpp Reading.cpp -std=c++17 -o main
