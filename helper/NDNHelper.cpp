//
// Created by mingj on 18-12-22.
//

#include "NDNHelper.h"

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
    cout << "registerIp: " << registerIp << endl;
    Name register_prefix1(NDNHelper::PREFIX_PRE_REQUEST + "/" + this->registerIp);
    Name register_prefix2(NDNHelper::PREFIX_REQUEST_DATA + "/" + this->registerIp);
    Name register_prefix3(NDNHelper::PREFIX_TCP_PRE_REQUEST + "/" + this->registerIp);
    Name register_prefix4(NDNHelper::PREFIX_TCP_REQUEST_DATA + "/" + this->registerIp);

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
        face.processEvents();
    } catch (exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}


/**
 * 初始化与NDN交互模块
 * @param configFilePath 配置文件的路径
 * @return
 */
void NDNHelper::initNDN(string configFilePath) {
    JSONCPPHelper jsoncppHelper(configFilePath);
    this->registerIp = jsoncppHelper.getString(NDNHelper::KEY_CONFIG_REGISTER_IP);
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


void NDNHelper::bindPendingInterestMap(MapCacheHelper<time_t> *pendingInterestMap) {
    this->pendingInterestMap = pendingInterestMap;
}

void NDNHelper::bindPrefixGuestTable(SetHelper<string> *prefixGuestTable) {
    this->prefixGuestTable = prefixGuestTable;
}

/**
 * 内部函数，处理onData事件
 * @param data
 */
void NDNHelper::dealOnData(const Data &data) {
    string name = data.getName().toUri();
    string pre = "/IP/pre/";
    if (name.find(pre, 0) != string::npos) {

    } else {        //正式拉取到包的回复
        vector<string> fileds;
        boost::split(fileds, name, boost::is_any_of("/"));
        this->rawSocketHelper.sendPacketTo(data.getContent().value(), data.getContent().value_size(), fileds[3]);
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
    if (isPre) {
		if (isTCP) {
			string next_name = "/IP/TCP";
		    vector<string> fileds;
		    boost::split(fileds, interest_name, boost::is_any_of("/"));

		    string sip = fileds[4];
		    string dip = fileds[5];
		    string uid = fileds[6];
		    next_name.append("/" + dip);
		    next_name.append("/" + sip);
		    next_name.append("/" + uid);

		    //发一个正式拉取的请求
		    this->expressInterest(next_name, false);
			prefixGuestTable -> erase(//删除已经发送这条)
			for(int i=0; i<NUM_OF_GUEST; i++) {
				//前缀猜测表
			}

		}
		else {
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
		    this->expressInterest(next_name, false);
		}
    } else {
		if (isTCP) {
			//悬而未决表
		}
		else {
		    string uuid = interest_name.substr(28, interest_name.length());
		    auto res = cacheHelper->get(uuid);
		    if (!res.second) {
		        cout << "没有找到uuid = " << uuid << "的数据包" << "(" << interest_name << ")" << endl;
		        return;
		    }
		    tuple_p tuple1 = res.first;

		    //删除
		    cacheHelper->erase(uuid);

		    Data data(interest_name);
		    data.setContent(tuple1->pkt, tuple1->size);
		    KeyChain_.sign(data);
		    this->face.put(data);
		}
    }
}

void NDNHelper::onData(const Interest &interest, const Data &data) {
    this->dealOnData(data);
}

void NDNHelper::onNack(const Interest &, const lp::Nack &) {

}


void NDNHelper::onTimeout(const Interest &interest, bool isPre) {
    if (!isPre) {
        cout << "Timed out: " << interest.getName().toUri() << endl;
    }
}

void NDNHelper::onInterest(const InterestFilter &filter, const Interest &interest, bool isPre, bool isTCP) {
    this->dealOnInterest(interest, isPre, isTCP);
}


void NDNHelper::onRegisterFailed(const Name &prefix) {
    cout << "Register failed for prefix " << prefix.toUri() << endl;
}

void NDNHelper::expressInterest(string name, bool isPre) {
    this->face.expressInterest(Interest(name), bind(&NDNHelper::onData, this, _1, _2),
                               bind(&NDNHelper::onNack, this, _1, _2), bind(&NDNHelper::onTimeout, this, _1, isPre));
}


void NDNHelper::putData(const string &name, const tuple_p tuple) {
    Data data(name);
    data.setContent(tuple->pkt, tuple->size);
    KeyChain_.sign(data);
    this->face.put(data);
}
