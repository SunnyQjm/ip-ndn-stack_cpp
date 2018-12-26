//
// Created by mingj on 18-12-22.
//

#ifndef IP_NDN_STACK_CPP_NDNHELPER_H
#define IP_NDN_STACK_CPP_NDNHELPER_H

#include <iostream>
#include "JSONCPPHelper.h"
#include "RawSocketHelper.h"
#include "CacheHelper.h"
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace ndn;
using namespace std;

class NDNHelper {
private:
    void dealOnData(const Data &data);
    void dealOnInterest(const Name &prefix, const Interest &interest, bool isPre);
public:
    NDNHelper();
    void initNDN(string configFilePath);
    void start();
    void join();
    void bindCacheHelper(CacheHelper* cacheHelper);
    void expressInterest(string name, bool isPre = true);

public: //回调
    void onData(const Interest &interest, const Data &data);

    void onNack(const Interest& interest, const lp::Nack& nack);
    void onTimeout(const Interest &interest, bool isPre = false);

    void onInterest(const Name &prefix,
                    const InterestFilter &filter,
                    const Interest &interest, bool isPre);

    void onRegisterFailed(const Name &prefix);

public: //静态变量
    static const string PREFIX_PRE_REQUEST;
    static const string PREFIX_REQUEST_DATA;
    static const string KEY_CONFIG_REGISTER_IP;
private:
    Face face;
    CacheHelper* cacheHelper;
    RawSocketHelper rawSocketHelper;
    pthread_t processEventThreadId;
    KeyChain KeyChain_;
    string registerIp;
};





#endif //IP_NDN_STACK_CPP_NDNHELPER_H
