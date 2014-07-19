###################################################################
#       CS570 masc1005
#       Makefile
###################################################################

TORM = prog2.o a2.o a2
CC = g++
CCFLAGS = -g -O3 -Wall -Wcast-qual -Wpointer-arith -Wwrite-strings

a2: a2.o prog2.o
        ${CC} ${CCFLAGS} -lrt -o a2 a2.o prog2.o

a2.o:   a2.cpp
        ${CC} ${CCFLAGS} -c a2.cpp

prog2.o:        prog2.cc prog2.h
        ${CC} ${CCFLAGS} -c prog2.cc

clean:
        rm -f ${TORM}