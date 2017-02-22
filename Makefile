#CC=musl-gcc
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -g -ansi
LDLIBS=-lm
SRC=traj.c testtraj.c
OBJS=traj.o testtraj.o
EXE=testtraj

$(EXE) : $(OBJS)

clean:
	rm -rf *.o
	rm -f $(EXE)
