#include <utility>

//
// Created by mingj on 18-12-22.
//

#include "NDNHelper.h"
#include <ndn-cxx/mgmt/nfd/controller.hpp>

//前缀
const string NDNHelper::PREFIX_PRE_REQUEST = "/IP/pre";
const string NDNHelper::PREFIX_REQUEST_DATA = "/IP";
const string NDNHelper::PREFIX_TCP_PRE_REQUEST = "/IP/TCP/pre";
const string NDNHelper::PREFIX_TCP_REQUEST_DATA = "/IP/TCP";

//配置文件的键值
const string NDNHelper::KEY_CONFIG_REGISTER_IP = "registerIp";

NDNHelper::NDNHelper() : face("localhost") {
    cout << "NDN Helper constructor" << endl;
}

void NDNHelper::start() {
    for(auto &ip : this->registerIp) {
        cout << "registerIp: " << ip << endl;
        Name register_prefix1(NDNHelper::PREFIX_PRE_REQUEST + "/" + ip);
        Name register_prefix2(NDNHelper::PREFIX_REQUEST_DATA + "/" + ip);
        Name register_prefix3(NDNHelper::PREFIX_TCP_PRE_REQUEST + "/" + ip);
        Name register_prefix4(NDNHelper::PREFIX_TCP_REQUEST_DATA + "/" + ip);
        Interest::setDefaultCanBePrefix(true);
        try {
            face.setInterestFilter(InterestFilter(register_prefix1),
                                   (const InterestCallback &) bind(&NDNHelper::onInterest, this, _1, _2, true, false),
                                   (const RegisterPrefixFailureCallback &) bind(&NDNHelper::onRegisterFailed, this, _1));

            face.setInterestFilter(InterestFilter(register_prefix2),
                                   (const InterestCallback &) bind(&NDNHelper::onInterest, this, _1, _2, false, false),
                                   (const RegisterPrefixFailureCallback &) bind(&NDNHelper::onRegisterFailed, this, _1));

            face.setInterestFilter(InterestFilter(register_prefix3),
                                   (const InterestCallback &) bind(&NDNHelper::onInterest, this, _1, _2, true, true),
                                   (const RegisterPrefixFailureCallback &) bind(&NDNHelper::onRegisterFailed, this, _1));

            face.setInterestFilter(InterestFilter(register_prefix4),
                                   (const InterestCallback &) bind(&NDNHelper::onInterest, this, _1, _2, false, true),
                                   (const RegisterPrefixFailureCallback &) bind(&NDNHelper::onRegisterFailed, this, _1));
        } catch (exception &e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
        }
    }
    face.processEvents();

//    Name register_prefix1(NDNHelper::PREFIX_PRE_REQUEST);
//    Name register_prefix2(NDNHelper::PREFIX_REQUEST_DATA);
//    Name register_prefix3(NDNHelper::PREFIX_TCP_PRE_REQUEST);
//    Name register_prefix4(NDNHelper::PREFIX_TCP_REQUEST_DATA);


}


/**
 * 初始化与NDN交互模块
 * @param configFilePath 配置文件的路径
 * @return
 */
void NDNHelper::initNDN(string configFilePath) {
    JSONCPPHelper jsoncppHelper(configFilePath);
    auto ips = jsoncppHelper.get(NDNHelper::KEY_CONFIG_REGISTER_IP);
    for(int i = 0; i < ips.size(); i++) {
        this->registerIp.push_back(ips[i].asString());
        cout << "register" << ips[i] << endl;
    }
//    this->registerIp = jsoncppHelper.getString(NDNHelper::KEY_CONFIG_REGISTER_IP);
}

/**
 * join thread
 */
void NDNHelper::join() {
    pthread_join(this->processEventThreadId, nullptr);
}


void NDNHelper::bindCacheHelper(MapCacheHelper<tuple_p> *cacheHelper) {
    this->cacheHelper = cacheHelper;
}


void NDNHelper::bindPendingInterestMap(MapCacheHelper<long> *pendingInterestMap) {
    this->pendingInterestMap = pendingInterestMap;
}

void NDNHelper::bindPrefixGuestTable(SetHelper<string> *prefixGuestTable) {
    this->prefixGuestTable = prefixGuestTable;
}

/**
 * 内部函数，处理onData事件
 * @param data
 */
void NDNHelper::dealOnData(const Data &data, bool isPre, bool isTCP) {
    string name = data.getName().toUri();
//    cout << "收到data包： " << name << endl;
    if(!isPre) {
        vector<string> fileds;
        boost::split(fileds, name, boost::is_any_of("/"));
        if(isTCP) {
            this->rawSocketHelper.sendPacketTo(data.getContent().value(), data.getContent().value_size(), fileds[4]);
        } else {
            this->rawSocketHelper.sendPacketTo(data.getContent().value(), data.getContent().value_size(), fileds[3]);
        }
    }
}

/**
 * 内部函数，处理onInterest事件
 * @param prefix
 * @param interest
 * @param face
 * @param isPre 是否是预请求
 */
void NDNHelper::dealOnInterest(const Interest &interest, bool isPre, bool isTCP) {
    string interest_name = interest.getName().toUri();
    //string pre = "/IP/pre/";
//    cout << "onInterest: " << interest_name << endl;

    // 只处理预请求，正式请求
    if (isPre) {
        if (isTCP) {
//            cout << "pre tcp" << endl;
            string next_name = "/IP/TCP";
            vector<string> fileds;
            boost::split(fileds, interest_name, boost::is_any_of("/"));

            string sip = fileds[4];
            string dip = fileds[5];
            string uid = fileds[6];
            next_name.append("/" + dip);
            next_name.append("/" + sip);
            string guess_name = next_name;

            next_name.append("/" + uid);

            //发一个正式拉取的请求
            this->expressInterest(next_name, false, true);

			if (this->prefixGuestTable->find(next_name)) {
            	this->prefixGuestTable->erase(next_name);   //删除已经发送这条
			}

            vector<string> uuid_fileds;
            boost::split(uuid_fileds, uid, boost::is_any_of("-"));
            int num_of_sequence = boost::lexical_cast<int>(uuid_fileds[2]);

            guess_name.append("/" + uuid_fileds[0] + "-" + uuid_fileds[1] + "-");
            for (int i = 0; i < NUM_OF_GUEST; i++) {
                string g_name = guess_name;
                g_name.append(to_string(++num_of_sequence));
                if (this->prefixGuestTable->saveConcurrence(g_name)) {
                    this->expressInterest(g_name, false, true);
                }
            }
        } else {
//            cout << "pre other" << endl;
            string next_name = "/IP";
            vector<string> fileds;
            boost::split(fileds, interest_name, boost::is_any_of("/"));

            string sip = fileds[3];
            string dip = fileds[4];
            string uid = fileds[5];
            next_name.append("/" + dip);
            next_name.append("/" + sip);
            next_name.append("/" + uid);

            //发一个正式拉取的请求
            this->expressInterest(next_name, false, false);
        }
    }
}

void NDNHelper::onData(const Interest &interest, const Data &data, bool isPre, bool isTCP) {
    this->dealOnData(data, isPre, isTCP);
}

void NDNHelper::onNack(const Interest &, const lp::Nack &) {

}


void NDNHelper::onTimeout(const Interest &interest, bool isPre) {
    if (!isPre) {
       // string next_name = interest.getName().toUri();
       // this->expressInterest(next_name, false, false);
//        cout << "Timed out: " << interest.getName().toUri() << endl;
    }
}

void NDNHelper::onInterest(const InterestFilter &filter, const Interest &interest, bool isPre, bool isTCP) {
    this->dealOnInterest(interest, isPre, isTCP);
}


void NDNHelper::onRegisterFailed(const Name &prefix) {
    cout << "Register failed for prefix " << prefix.toUri() << endl;
}

void NDNHelper::expressInterest(string name, bool isPre, bool isTCP) {
	Interest interest(name);
	interest.setInterestLifetime(2_s);	//兴趣报存活时间
	cout << "express interest: " << name << endl;
    this->face.expressInterest(interest, bind(&NDNHelper::onData, this, _1, _2, isPre, isTCP),
                               bind(&NDNHelper::onNack, this, _1, _2), bind(&NDNHelper::onTimeout, this, _1, isPre));
}


void NDNHelper::putData(const string &name, const tuple_p tuple) {
    Data data(name);
    data.setContent(tuple->pkt, tuple->size);
    KeyChain_.sign(data);
    this->face.put(data);
}

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
* @return   <前缀，uuid>
*/
pair<string, string>  NDNHelper::buildName(uint32_t sip, uint32_t dip, uint16_t sport, uint16_t dport, int type, int seq,
                            string uid) {

    //网络字节序转主机字节序
    sip = ntohl(sip);
    dip = ntohl(dip);
    sport = ntohs(sport);
    dport = ntohs(dport);

    //得到source ip
    string sourceIP = to_string((sip >> 24) & 0xFF);
    sourceIP.append(".");
    sourceIP.append(to_string((sip >> 16) & 0xFF));
    sourceIP.append(".");
    sourceIP.append(to_string((sip >> 8) & 0xFF));
    sourceIP.append(".");
    sourceIP.append(to_string((sip >> 0) & 0xFF));

    //得到目的 ip
    string dstIP = to_string((dip >> 24) & 0xFF);
    dstIP.append(".");
    dstIP.append(to_string((dip >> 16) & 0xFF));
    dstIP.append(".");
    dstIP.append(to_string((dip >> 8) & 0xFF));
    dstIP.append(".");
    dstIP.append(to_string((dip >> 0) & 0xFF));

    //得到端口号
    string sourcePort = to_string(sport);
    string dstPort = to_string(dport);


    switch (type) {
        case 1:
            return make_pair(NDNHelper::PREFIX_PRE_REQUEST + "/" + dstIP + "/" + sourceIP + "/" + uid, uid);
        case 2:
            return make_pair(NDNHelper::PREFIX_REQUEST_DATA + "/" + sourceIP + "/" + dstIP + "/" + uid, uid);
        case 3:
            uid = sourcePort + "-" + dstPort +
                  "-" + to_string(seq);
            return make_pair(NDNHelper::PREFIX_TCP_PRE_REQUEST + "/" + dstIP + "/" + sourceIP + "/" + uid, uid);
        case 4:
            uid = sourcePort + "-" + dstPort +
                  "-" + to_string(seq);
            return make_pair(NDNHelper::PREFIX_TCP_REQUEST_DATA + "/" + sourceIP + "/" + dstIP + "/" + uid, uid);
        default:
            return make_pair("", "");
    }
}

string NDNHelper::build4TupleKey(uint32_t sip, uint32_t dip, uint16_t sport, uint16_t dport) {
    //网络字节序转主机字节序
    sip = ntohl(sip);
    dip = ntohl(dip);
    sport = ntohs(sport);
    dport = ntohs(dport);

    //得到source ip
    string sourceIP = to_string((sip >> 24) & 0xFF);
    sourceIP.append(".");
    sourceIP.append(to_string((sip >> 16) & 0xFF));
    sourceIP.append(".");
    sourceIP.append(to_string((sip >> 8) & 0xFF));
    sourceIP.append(".");
    sourceIP.append(to_string((sip >> 0) & 0xFF));

    //得到目的 ip
    string dstIP = to_string((dip >> 24) & 0xFF);
    dstIP.append(".");
    dstIP.append(to_string((dip >> 16) & 0xFF));
    dstIP.append(".");
    dstIP.append(to_string((dip >> 8) & 0xFF));
    dstIP.append(".");
    dstIP.append(to_string((dip >> 0) & 0xFF));

    //得到端口号
    string sourcePort = to_string(sport);
    string dstPort = to_string(dport);

    return sourceIP + "/" + dstIP + "/" + sourcePort + "/" + dstPort;
}

long NDNHelper::getCurTime() {
    auto duration_in_ms = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now()
            .time_since_epoch());
    return duration_in_ms.count();
}

void NDNHelper::putDataToCache(const string &interestName, tuple_p tuple) {
    Data data(interestName);
    data.setContent(tuple->pkt, tuple->ipSize);
//    {
//        boost::unique_lock<boost::shared_mutex> m(signMutex);
        KeyChain_.sign(data);
//    }
    this->face.put(data);
}
