//
// Created by mingj on 18-12-25.
//

#ifndef IP_NDN_STACK_CPP_CACHEHELPER_H
#define IP_NDN_STACK_CPP_CACHEHELPER_H

#include "../libpcapcapture/libpcap.h"
#include <unordered_map>
#include <iostream>
#include <boost/thread/pthread/shared_mutex.hpp>

using namespace std;

class CacheHelper {
public:
    CacheHelper(){}
    bool save(string key, tuple_p value);
    pair<tuple_p, bool> get(string key);
    unsigned int erase(string key);
private:
    unordered_map<string, tuple_p> ipPacketCache;
    tuple_p emptyTuple;
    boost::shared_mutex mutex;
};


#endif //IP_NDN_STACK_CPP_CACHEHELPER_H
