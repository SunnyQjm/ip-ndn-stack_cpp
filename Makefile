PROGS = main

SRC += libpcapcapture/config.h libpcapcapture/config.cpp libpcapcapture/packet.cpp libpcapcapture/ttime.cpp libpcapcapture/util.cpp  \
    libpcapcapture/ringbuffer.cpp libpcapcapture/hash.cpp helper/NDNHelper.cpp \
	helper/RawSocketHelper.cpp

OBJS = main.o

CFLAGS = -Wall -std=c++14 #-fno-omit-frame-pointer

DEFAULT_LINK_LIBS = -lrt -liniparser -lm  -lpcap -lpthread -lndn-cpp

#CFLAGS += -D APPROXIMATE
#CFLAGS += -D TRACE_FILE
#CFLAGS += -D TRACE_DIR
#CFLAGS += -D TRACE_FILE_RAM
#CFLAGS += -D TRACE_DIE_RAM
CFLAGS += -D HAVE_PF_RING



all: ${PROGS}

main: $(SRC) main.o
	g++ -g $(CFLAGS) -o $@ $^ ${DEFAULT_LINK_LIBS}

clean:
	rm -f ${PROGS} ${OBJS}
