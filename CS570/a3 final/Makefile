###################################################################
#       CS570 masc1005
#       Makefile
###################################################################

TORM = prog3.o a3.o a3
CC = g++
CCFLAGS = -g -O3 -Wall -Wcast-qual -Wpointer-arith -Wwrite-strings

a3: a3.o prog3.o
        ${CC} ${CCFLAGS} -lrt -o a3 a3.o prog3.o

a3.o:   a3.cpp
        ${CC} ${CCFLAGS} -c a3.cpp

prog3.o:        prog3.cc prog3.h
        ${CC} ${CCFLAGS} -c prog3.cc

clean:
        rm -f ${TORM}