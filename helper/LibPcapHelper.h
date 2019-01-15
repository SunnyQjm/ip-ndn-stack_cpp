//
// Created by mingj on 18-12-23.
//

#ifndef IP_NDN_STACK_CPP_LIBPCAPHELPER_H
#define IP_NDN_STACK_CPP_LIBPCAPHELPER_H

#include <iostream>
#include "RawSocketHelper.h"
#include "JSONCPPHelper.h"
#include "NDNHelper.h"
#include "MapCacheHelper.h"
#include "PcapHelper.h"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <ndn-cxx/face.hpp>
#include <sys/wait.h>
#include <errno.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "ThreadPool.h"


using namespace std;
using namespace ndn;
using namespace IP_NDN_STACK::pcap;
class LibPcapHelper {
public:
    explicit
    LibPcapHelper(const string &configFilePath);

    void start();

    void close();

    string generateUUID();

    void bindNDNHelper(NDNHelper *ndnHelper);

    /**
     * 绑定缓存表
     * @param cacheHelper
     */
    void bindCacheHelper(MapCacheHelper<tuple_p> *cacheHelper);

    /**
     * 绑定悬而未决表
     * @param pendingInterestMap
     */
    void bindPendingInterestTable(MapCacheHelper<long> *pendingInterestMap);

    /**
     * 绑定自增序列号表
     * @param sequenceTable
     */
    void bindSequenceTable(MapCacheHelper<int> *sequenceTable);

protected:
    void
    asyncRead();

    void
    handleRead(const boost::system::error_code &error);

    void
    handleError(const std::string &errorMessage);

private:
    PcapHelper mPcap;
    boost::asio::io_service service;
    boost::asio::posix::stream_descriptor m_socket;
    NDNHelper *ndnHelper;
    MapCacheHelper<tuple_p> *cacheHelper;               //缓存表
    MapCacheHelper<long> *pendingInterestTable;       //悬而未决表
    /**
     * key -> 四元组
     * value -> 代表最后一次处理的该四元组对应的包的编码
     */
    MapCacheHelper<int> *sequenceTable;                 //自增序列号表
    RawSocketHelper rawSocketHelper;

    static const int THREAD_POOL_SIZE = 1;

    ThreadPool threadPool;

    void deal(tuple_p tuple);

};


#endif //IP_NDN_STACK_CPP_LIBPCAPHELPER_H
