PROGS = raw_socket_test json_cpp_test test_split

OBJS = raw_socket_test.o json_cpp_test.o test_split.o

DEFAULT_LINK_LIBS = -std=c++14 -ljsoncpp

all: ${PROGS}

raw_socket_test: raw_socket_test.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}
json_cpp_test: json_cpp_test.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}
test_split: test_split.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}

clean:
	rm -f ${PROGS} ${OBJS}

