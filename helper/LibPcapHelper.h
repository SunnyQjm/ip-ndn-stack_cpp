//
// Created by mingj on 18-12-23.
//

#ifndef IP_NDN_STACK_CPP_LIBPCAPHELPER_H
#define IP_NDN_STACK_CPP_LIBPCAPHELPER_H

#include <iostream>
#include "../libpcapcapture/libpcap.h"
#include "RawSocketHelper.h"
#include "JSONCPPHelper.h"
#include "NDNHelper.h"
#include "CacheHelper.h"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <ndn-cpp/face.hpp>
#include <sys/wait.h>
#include <errno.h>
#include <boost/thread.hpp>

using namespace std;
using namespace ndn;

class LibPcapHelper {
public:
    LibPcapHelper();

    void initLibPcap(string configFilePath);

    void close();

    string generateUUID();

    void bindNDNHelper(NDNHelper* ndnHelper);

    void bindCacheHelper(CacheHelper* cacheHelper);

    void join();

private:
    pcap_t *ph;
    NDNHelper *ndnHelper;
    CacheHelper *cacheHelper;
    RawSocketHelper rawSocketHelper;
//    void deal(tuple_p tuple);
    void deal(const void *arg1, const void *arg2);
};


#endif //IP_NDN_STACK_CPP_LIBPCAPHELPER_H
