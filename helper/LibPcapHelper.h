//
// Created by mingj on 18-12-23.
//

#ifndef IP_NDN_STACK_CPP_LIBPCAPHELPER_H
#define IP_NDN_STACK_CPP_LIBPCAPHELPER_H

#include <iostream>
#include "../libpcapcapture/ndn.cpp"
#include "JSONCPPHelper.h"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <ndn-cpp/face.hpp>

using namespace std;
class LibPcapHelper {
public:
    LibPcapHelper();
    void initLibPcap(string configFilePath);
    void close();
    string generateUUID();
    void join();
private:
    pcap_t *ph;
    pthread_t readRingBufferThreadId;
    Face face;
    void deal(tuple_t tuple);
};


#endif //IP_NDN_STACK_CPP_LIBPCAPHELPER_H
