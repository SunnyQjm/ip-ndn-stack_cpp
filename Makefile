PROGS = main

SRC += libpcapcapture/packet.cpp libpcapcapture/ttime.cpp   \
    helper/JSONCPPHelper.cpp helper/RawSocketHelper.cpp helper/NDNHelper.cpp \
    helper/LibPcapHelper.cpp helper/CacheHelper.cpp

OBJS = main.o

CFLAGS = -Wall -std=c++14 #-fno-omit-frame-pointer

DEFAULT_LINK_LIBS = -lrt -lm  -lpcap -lpthread -lndn-cxx -ljsoncpp -lboost_system -lboost_thread

#CFLAGS += -D APPROXIMATE
#CFLAGS += -D TRACE_FILE
#CFLAGS += -D TRACE_DIR
#CFLAGS += -D TRACE_FILE_RAM
#CFLAGS += -D TRACE_DIE_RAM
CFLAGS += -D HAVE_PF_RING



all: ${PROGS}

main: $(SRC) main.cpp
	g++ -g $(CFLAGS) -o $@ $^ ${DEFAULT_LINK_LIBS}

clean:
	rm -f ${PROGS} ${OBJS}
