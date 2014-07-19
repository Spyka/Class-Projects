###################################################################
#       CS570 masc1005
#       makefile
###################################################################

TORM = prog1.o sem.o sem
CC = g++
CCFLAGS = -g -O3 -Wall -Wcast-qual -Wpointer-arith -Wwrite-strings

sem: sem.o prog1.o
        ${CC} ${CCFLAGS} -lrt -o sem sem.o prog1.o

sem.o:  sem.cpp
        ${CC} ${CCFLAGS} -c sem.cpp

prog1.o:        prog1.cc prog1.h
        ${CC} ${CCFLAGS} -c prog1.cc

clean:
        rm -f ${TORM}