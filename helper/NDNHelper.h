//
// Created by mingj on 18-12-22.
//

#ifndef IP_NDN_STACK_CPP_NDNHELPER_H
#define IP_NDN_STACK_CPP_NDNHELPER_H

#include <iostream>
#include "JSONCPPHelper.h"
#include "RawSocketHelper.h"
#include "MapCacheHelper.h"
#include "SetHelper.h"
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

    void dealOnInterest(const Interest &interest, bool isPre);

public:
    NDNHelper();

    void initNDN(string configFilePath);

    void start();

    void join();

    /**
     * 绑定一个LibPcapHelper模块实例
     * @param libPcapHelper
     */
    void bindCacheHelper(MapCacheHelper<tuple_p > *cacheHelper);

    /**
     * 绑定一个悬而未决表
     * @param pendingInterestMap
     */
    void bindPendingInterestMap(MapCacheHelper<time_t> *pendingInterestMap);

    /**
     * 绑定一个前缀猜测表
     * @param prefixGuestTable
     */
    void bindPrefixGuestTable(SetHelper<string> *prefixGuestTable);

    /**
     * 发送一个数据包
     * @param name
     * @param isPre
     */
    void expressInterest(string name, bool isPre = true);

    /**
     * 直接发送一个data包，响应一个pending兴趣包
     * @param name    data包的名字（要响应的兴趣包的名字）
     * @param tuple
     */
    void putData(const string &name, tuple_p tuple);

public: //回调
    void onData(const Interest &interest, const Data &data);

    void onNack(const Interest &interest, const lp::Nack &nack);

    void onTimeout(const Interest &interest, bool isPre = false);

    void onInterest(const InterestFilter &filter,
                    const Interest &interest, bool isPre);

    void onRegisterFailed(const Name &prefix);

public: //静态变量
    static const string PREFIX_PRE_REQUEST;
    static const string PREFIX_REQUEST_DATA;
    static const string PREFIX_TCP_PRE_REQUEST;
    static const string PREFIX_TCP_REQUEST_DATA;
    static const string KEY_CONFIG_REGISTER_IP;
	static const int NUM_OF_GUEST = 20;
private:
    Face face;
    MapCacheHelper<tuple_p> *cacheHelper;            //缓存表
    MapCacheHelper<time_t> *pendingInterestMap;     //悬而未决表
    SetHelper<string> *prefixGuestTable;            //前缀猜测表
    RawSocketHelper rawSocketHelper;
    pthread_t processEventThreadId;
    KeyChain KeyChain_;
    string registerIp;
};


#endif //IP_NDN_STACK_CPP_NDNHELPER_H
