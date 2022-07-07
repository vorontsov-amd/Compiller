CC=g++

CFLAGS=-c

all: data_t.cpp Differenciator.cpp lexical_anaysis.cpp main.cpp Reading.cpp Compiliter.cpp
	g++ main.cpp data_t.cpp Differenciator.cpp lexical_anaysis.cpp Reading.cpp Compiliter.cpp -std=c++17 -o main
