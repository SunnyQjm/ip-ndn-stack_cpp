PROGS = raw_socket_test json_cpp_test test_split test_uuid

OBJS = raw_socket_test.o json_cpp_test.o test_split.o test_uuid.o

DEFAULT_LINK_LIBS = -std=c++14 -ljsoncpp

all: ${PROGS}

raw_socket_test: raw_socket_test.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}
json_cpp_test: json_cpp_test.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}
test_split: test_split.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}
test_uuid: test_uuid.o
	g++ -o $@ $@.o ${DEFAULT_LINK_LIBS}

clean:
	rm -f ${PROGS} ${OBJS}

