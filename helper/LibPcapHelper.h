//
// Created by mingj on 18-12-23.
//

#ifndef IP_NDN_STACK_CPP_LIBPCAPHELPER_H
#define IP_NDN_STACK_CPP_LIBPCAPHELPER_H

#include <iostream>
#include "../libpcapcapture/ndn.cpp"
#include "JSONCPPHelper.h"

using namespace std;
class LibPcapHelper {
public:
    LibPcapHelper();
    void initLibPcap(string configFilePath);
    void close();
    void join();
private:
    pcap_t *ph;
    pthread_t readRingBufferThreadId;
};


#endif //IP_NDN_STACK_CPP_LIBPCAPHELPER_H
