CPP = g++

EXEC += mworker

all: $(EXEC)

CFLAGS = -Wall -std=c++11 #-fno-omit-frame-pointer
CFLAGS += -D HAVE_PF_RING
#CFLAGS += -D APPROXIMATE
#CFLAGS += -D TRACE_FILE
#CFLAGS += -D TRACE_DIR
#CFLAGS += -D TRACE_FILE_RAM
#CFLAGS += -D TRACE_DIE_RAM

#HEADER += alg_keytbl.h alg_prob_array.h config.h  packet.h ringbuffer.h tuple.h util.h massdal.h ttime.h hash.h  

SRC += config.cpp packet.cpp ttime.cpp util.cpp  ringbuffer.cpp hash.cpp 
LIBS = -lrt -liniparser 
#VPATH=HEADER:SRC:LIBS
mworker: mworker.cpp $(SRC)
	g++ -g $(CFLAGS) -o $@ $< $(SRC) $(LIBS) -lm  -lpcap -lpthread -lndn-cpp

clean:
	rm -rf $(EXEC)
	rm -rf *log*
	rm -rf *out*
