PROGS = main

OBJS = main.o

DEFAULT_LINK_LIBS = -std=c++14

all: ${PROGS}

main: main.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}

clean:
	rm -f ${PROGS} ${OBJS}

