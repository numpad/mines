
CWARN=-Wall -pedantic -ggdb
CSTD=-std=c++14
CLIB=-lm -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ ${CSTD} ${CWARN} ${CLIB} *.cpp -omain
