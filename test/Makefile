PROGS = raw_socket_test

OBJS = raw_socket_test.o

DEFAULT_LINK_LIBS = -std=c++14

all: ${PROGS}

raw_socket_test: raw_socket_test.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}

clean:
	rm -f ${PROGS} ${OBJS}

