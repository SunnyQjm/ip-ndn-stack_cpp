//
// Created by mingj on 18-12-23.
//

#include "LibPcapHelper.h"

LibPcapHelper::LibPcapHelper(const string &configFilePath): mPcap(configFilePath), m_socket(service) {
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
    cout << "handle read" << endl;
    auto res = mPcap.readNextPacketAfterDecode();
    this->deal(std::get<0>(res));
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
    if (tuple->key.proto == IPPROTO_TCP) {
        string key = ndnHelper->build4TupleKey(tuple->key.src_ip, tuple->key.dst_ip,
                tuple->key.src_port, tuple->key.dst_port);

        auto res = sequenceTable->get(key);

        if (!res.second) {//若不存在则将index即自增表的value设为1并插入；再存入缓存中
            tuple->index = 1;
            auto result_seq = sequenceTable->save(key, tuple->index);
            if (!result_seq) {
                cout << "插入失败" << endl;
                return;
            }
            auto prefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                   tuple->key.src_port, tuple->key.dst_port, 3, 1);
            string uuid = prefixUUID.second;
            cout << prefixUUID.first << endl;

            auto result_cache = cacheHelper->save(uuid, tuple);
            if (!result_cache) {
                cout << "插入失败" << endl;
                return;
            }
            ndnHelper->expressInterest(prefixUUID.first);
            return;
        } else {//若存在则将index的值++，并查找悬而未决表
            if (!this->sequenceTable->getAndIncreaseSequence(key, tuple)) {
                cout << "获取自增序列失败" << endl;
                return;
            }

            auto prefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                   tuple->key.src_port, tuple->key.dst_port, 4, tuple->index);
            //res.second = tuple->index;
            string uuid = prefixUUID.second;
            string formal_name = prefixUUID.first;

            auto formal_res = pendingInterestTable->get(formal_name);
            pendingInterestTable->erase(formal_name);          //删除相应悬而未决表表项
            long curTime = ndnHelper->getCurTime();
            if (formal_res.second && formal_res.first >= curTime) {     //如果找到相应表项，若时间在有效期内，则直接发送date包
//                cout << "找到相应表项，若时间在有效期内，则直接发送date包" << endl;
                ndnHelper->putData(formal_name, tuple);
            } else {                                                    //未找到或则时间失效则将数据进行缓存并发送预请求兴趣包并删除相应表项
                cacheHelper->save(uuid, tuple);

                //发送预请求兴趣包
                auto prePrefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                                          tuple->key.src_port, tuple->key.dst_port, 3, tuple->index);

                ndnHelper->expressInterest(prePrefixUUID.first, true);
                return;
            }
        }
        //	tuple_p tuple1 = res.first;
    } else {//为其他协议包用原来的方式传输
        string uuid = this->generateUUID();
        auto prefixUUID = ndnHelper->buildName(tuple->key.src_ip, tuple->key.dst_ip,
                                               tuple->key.src_port, tuple->key.dst_port, 1, -1, uuid);
        auto result = cacheHelper->save(uuid, tuple);
        if (!result) {
            cout << "插入失败" << endl;
            return;
        }

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



