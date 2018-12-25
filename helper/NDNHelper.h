//
// Created by mingj on 18-12-22.
//

#ifndef IP_NDN_STACK_CPP_NDNHELPER_H
#define IP_NDN_STACK_CPP_NDNHELPER_H

#include <iostream>
#include "JSONCPPHelper.h"
#include "RawSocketHelper.h"
#include "CacheHelper.h"
#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
using namespace ndn;
using namespace ndn::func_lib;
using namespace std;



class NDNHelper {
private:
    void dealOndata(const ptr_lib::shared_ptr<Data> &data);
    void dealOnInterest(const ptr_lib::shared_ptr<const Name> &prefix, const ptr_lib::shared_ptr<const Interest> &interest,
                        Face &face, bool isPre);
public:
    NDNHelper();
    void initNDN(string configFilePath);
    void join();
    void bindCacheHelper(CacheHelper cacheHelper);
    void expressInterest(string name);

public: //回调
    void onData(const ptr_lib::shared_ptr<const Interest> &interest, const ptr_lib::shared_ptr<Data> &data);

    void onTimeout(const ptr_lib::shared_ptr<const Interest> &interest);

    void onInterest(const ptr_lib::shared_ptr<const Name> &prefix,
                    const ptr_lib::shared_ptr<const Interest> &interest, Face &face,
                    uint64_t interestFilterId,
                    const ptr_lib::shared_ptr<const InterestFilter> &filter, bool isPre);

    void onRegisterFailed(const ptr_lib::shared_ptr<const Name> &prefix);

public: //静态变量
    static const string PREFIX_PRE_REQUEST;
    static const string PREFIX_REQUEST_DATA;
    static const string KEY_CONFIG_REGISTER_IP;
private:
    Face face;
    CacheHelper cacheHelper;
    RawSocketHelper rawSocketHelper;
    pthread_t processEventThreadId;
};





#endif //IP_NDN_STACK_CPP_NDNHELPER_H
