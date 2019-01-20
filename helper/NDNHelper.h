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
#include <boost/lexical_cast.hpp>
#include <vector>
#include <chrono>
#include <boost/thread/pthread/shared_mutex.hpp>


using namespace ndn;
using namespace std;

class NDNHelper {
private:
    void dealOnData(const Data &data, bool isPre, bool isTCP);

    void dealOnInterest(const Interest &interest, bool isPre, bool isTCP);

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
    void bindPendingInterestMap(MapCacheHelper<long> *pendingInterestMap);

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
    void expressInterest(string name, bool isPre = true, bool isTCP = false);

    /**
     * 直接发送一个data包，响应一个pending兴趣包
     * @param name    data包的名字（要响应的兴趣包的名字）
     * @param tuple
     */
    void putData(const string &name, tuple_p tuple);


    /**
     * 构造前缀
     * @param sip
     * @param dip
     * @param sport
     * @param dport
     * @param type
     *          type = 1    => /IP/pre
     *          type = 2    => /IP
     *          type = 3    => /IP/TCP/pre
     *          type = 4    => /IP/TCP
     * @return <前缀, uuid>
     */
    pair<string, string> buildName(uint32_t sip, uint32_t dip, uint16_t sport, uint16_t dport, int type, int seq = -1, string uid = "");

    /**
     * 够着四元组组成的key
     * @param sip
     * @param dip
     * @param sport
     * @param dport
     * @return
     */
    string build4TupleKey(uint32_t sip, uint32_t dip, uint16_t sport, uint16_t dport);

    /**
     * 获取当前时间戳
     */
    long getCurTime();

    /**
     * 将IP包放到NFD的缓存当中
     * @param interestName
     * @param tuple
     */
    void putDataToCache(const string &interestName, tuple_p tuple);

public: //回调
    void onData(const Interest &interest, const Data &data, bool isPre, bool isTCP);

    void onNack(const Interest &interest, const lp::Nack &nack);

    void onTimeout(const Interest &interest, bool isPre = false);

    void onInterest(const InterestFilter &filter,
                    const Interest &interest, bool isPre, bool isTCP);

    void onRegisterFailed(const Name &prefix);



public: //静态变量
    static const string PREFIX_PRE_REQUEST;
    static const string PREFIX_REQUEST_DATA;
    static const string PREFIX_TCP_PRE_REQUEST;
    static const string PREFIX_TCP_REQUEST_DATA;
    static const string KEY_CONFIG_REGISTER_IP;
	static const int NUM_OF_GUEST = 60;
private:
    Face face;
    MapCacheHelper<tuple_p> *cacheHelper;            //缓存表
    MapCacheHelper<long> *pendingInterestMap;     //悬而未决表
    SetHelper<string> *prefixGuestTable;            //前缀猜测表
    RawSocketHelper rawSocketHelper;
    pthread_t processEventThreadId;
    KeyChain KeyChain_;
    vector<string> registerIp;
    boost::shared_mutex signMutex;

};


#endif //IP_NDN_STACK_CPP_NDNHELPER_H
