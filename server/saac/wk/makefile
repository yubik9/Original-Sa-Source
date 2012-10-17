CC=gcc

all : acwk


acwk : wk.c TCPIP.o error.o tools.o
	gcc -o acwk -O -g -Wall wk.c error.o TCPIP.o tools.o

TCPIP:
error:
tools:

clean :
	rm -f acwk *.o *~


