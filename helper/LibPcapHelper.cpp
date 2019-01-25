//
// Created by mingj on 18-12-23.
//

#include "LibPcapHelper.h"


LibPcapHelper::LibPcapHelper(const string &configFilePath)
        : mPcap(configFilePath), m_socket(service), threadPool(THREAD_POOL_SIZE) {
    JSONCPPHelper jsoncppHelper(configFilePath);
    string filter = jsoncppHelper.getString("pcap_dstmac");
    try {
        mPcap.activate();
        mPcap.setPacketFilter(filter);
        m_socket.assign(mPcap.getFd());
    } catch (const PcapHelper::Error &e) {
        BOOST_THROW_EXCEPTION(PcapHelper::Error(e.what()));
    }
    asyncRead();
}

void LibPcapHelper::bindNDNHelper(NDNHelper *ndnHelper) {
    this->ndnHelper = ndnHelper;
}

void LibPcapHelper::bindCacheHelper(MapCacheHelper<tuple_p> *cacheHelper) {
    this->cacheHelper = cacheHelper;
}

void LibPcapHelper::bindPendingInterestTable(MapCacheHelper<long> *pendingInterestMap) {
    this->pendingInterestTable = pendingInterestMap;
}


void LibPcapHelper::bindSequenceTable(MapCacheHelper<int> *sequenceTable) {
    this->sequenceTable = sequenceTable;
}


void LibPcapHelper::asyncRead() {
    m_socket.async_read_some(boost::asio::null_buffers(),
                             [this](const auto &e, auto) { this->handleRead(e); });
}


void LibPcapHelper::handleRead(const boost::system::error_code &error) {
    if (error) {
        cout << "error: " << error;
        return;
    }
    auto res = mPcap.readNextPacketAfterDecode();
    auto tuple = std::get<0>(res);
    if (tuple != nullptr && tuple->ipSize < 8600) {
        this->deal(tuple);
//        //放入线程池中执行
//        threadPool.enqueue([tuple](LibPcapHelper * libPcapHelper) {
//            libPcapHelper->deal(tuple);
//        }, this);
    }
    asyncRead();
}

void LibPcapHelper::handleError(const std::string &errorMessage) {
    cerr << "ERROR: " << errorMessage << endl;
    close();
}

void LibPcapHelper::start() {
    cout << "start" << endl;
    service.run();
}

void LibPcapHelper::close() {
    //free the pcap handler
    if (m_socket.is_open()) {
        // Cancel all outstanding operations and close the socket.
        // Use the non-throwing variants and ignore errors, if any.
        boost::system::error_code error;
        m_socket.cancel(error);
        m_socket.close(error);
    }
    mPcap.close();
}


/**
 * 抓到包，并解析后，在此函数中处理
 * @param tuple
 */
void LibPcapHelper::deal(tuple_p tuple) {

    if (tuple->key.proto == IPPROTO_TCP || tuple->key.proto == IPPROTO_UDP) {
        string key = ndnHelper->build4TupleKey(tuple->key.src_ip, tuple->key.dst_ip,
                                               tuple->key.src_port, tuple->key.dst_port);

        auto res = sequenceTable->get(key);

        if (!res.second) {  //若不存在则将index即自增表的value设为1并插入；再存入缓存中
            tuple->index = 1;
            auto result_seq = sequenceTable->save(key, tuple->index);
            if (!result_seq) {
                cout << "插入失败" << endl;
                return;
            }

            auto dataPrefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                       tuple->key.src_port, tuple->key.dst_port, 4, tuple->index);

            ndnHelper->putDataToCache(dataPrefixUUID.first, tuple);

            auto prefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                   tuple->key.src_port, tuple->key.dst_port, 3, 1);
            ndnHelper->expressInterest(prefixUUID.first);
            return;
        } else {//若存在则将index的值++，并查找悬而未决表
            if (!this->sequenceTable->getAndIncreaseSequence(key, tuple)) {
                cout << "获取自增序列失败" << endl;
                return;
            }
            auto dataPrefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                       tuple->key.src_port, tuple->key.dst_port, 4, tuple->index);

            ndnHelper->putDataToCache(dataPrefixUUID.first, tuple);

            //发送预请求兴趣包
            auto prePrefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                      tuple->key.src_port, tuple->key.dst_port, 3, tuple->index);
            ndnHelper->expressInterest(prePrefixUUID.first, true);
            return;
        }
        //	tuple_p tuple1 = res.first;
    } else {//为其他协议包用原来的方式传输
        string uuid = this->generateUUID();

        auto dataPrefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                   tuple->key.src_port, tuple->key.dst_port, 2, -1, uuid);

        ndnHelper->putDataToCache(dataPrefixUUID.first, tuple);

        auto prefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                               tuple->key.src_port, tuple->key.dst_port, 1, -1, uuid);

        ndnHelper->expressInterest(prefixUUID.first);
    }

}

/**
 * 随机生成一个uuid
 * @return
 */
string LibPcapHelper::generateUUID() {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); // 这里是两个() ，因为这里是调用的 () 的运算符重载
    const string tmp_uuid = boost::uuids::to_string(a_uuid);
    return tmp_uuid;
}



