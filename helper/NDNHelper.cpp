//
// Created by mingj on 18-12-22.
//

#include "NDNHelper.h"

//前缀
const string NDNHelper::PREFIX_PRE_REQUEST = "/IP/pre";
const string NDNHelper::PREFIX_REQUEST_DATA = "/IP";
//配置文件的键值
const string NDNHelper::KEY_CONFIG_REGISTER_IP = "registerIp";

NDNHelper::NDNHelper(): face("localhost") {
    cout << "NDN Helper constructor" << endl;
}

///**
// * 在子线程中处理循环处理事件
// * @param arg
// * @return
// */
//void *dealEvent(void *arg) {
//    Face * face = (Face *) arg;
//    string register_prefix1_str(NDNHelper::PREFIX_PRE_REQUEST);
//    register_prefix1_str.append("/");
//    register_prefix1_str.append(registerIp);
//    string register_prefix2_str(NDNHelper::PREFIX_REQUEST_DATA);
//    register_prefix2_str.append("/");
//    register_prefix2_str.append(registerIp);
//    Name register_prefix1(register_prefix1_str);
//    Name register_prefix2(register_prefix2_str);
//
//    face->setInterestFilter(register_prefix1)
//}


void NDNHelper::start() {
//    string register_prefix1_str(NDNHelper::PREFIX_PRE_REQUEST);
//    register_prefix1_str.append("/");
//    register_prefix1_str.append(registerIp);
//    string register_prefix2_str(NDNHelper::PREFIX_REQUEST_DATA);
//    register_prefix2_str.append("/");
//    register_prefix2_str.append(registerIp);
    Name register_prefix1(NDNHelper::PREFIX_PRE_REQUEST + "/" + this->registerIp);
    Name register_prefix2(NDNHelper::PREFIX_REQUEST_DATA + "/" + this->registerIp);

    Interest::setDefaultCanBePrefix(true);
    try {
        face.setInterestFilter(InterestFilter(register_prefix1), (const InterestCallback &) bind(&NDNHelper::onInterest, this, register_prefix1, _1, _2, true),
                               (const RegisterPrefixFailureCallback&) bind(&NDNHelper::onRegisterFailed, this, _1));

        face.setInterestFilter(InterestFilter(register_prefix2), (const InterestCallback &) bind(&NDNHelper::onInterest, this, register_prefix1, _1, _2, true),
                               (const RegisterPrefixFailureCallback&) bind(&NDNHelper::onRegisterFailed, this, _1));
//        face.setInterestFilter(InterestFilter(register_prefix2), bind(&NDNHelper::onInterest, this, _1, _2, _3, _4, _5, true),
//                               bind(&NDNHelper::onRegisterFailed, this, _1));

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
    cout << "registerIp: " << registerIp << endl;
    // 前缀注册
//    this->face.setCommandSigningInfo(KeyChain_, KeyChain_.getDefaultCertificateName());
//    string register_prefix1_str(PREFIX_PRE_REQUEST);
//    register_prefix1_str.append("/");
//    register_prefix1_str.append(registerIp);
//    string register_prefix2_str(PREFIX_REQUEST_DATA);
//    register_prefix2_str.append("/");
//    register_prefix2_str.append(registerIp);
//    Name register_prefix1(register_prefix1_str);
//    Name register_prefix2(register_prefix2_str);

//    this->face.registerPrefix(register_prefix1,
//                              (const OnInterestCallback &) bind(&NDNHelper::onInterest, this, _1, _2, _3, _4, _5, true),
//                              bind(&NDNHelper::onRegisterFailed, this, _1));
//    this->face.processEvents();
//
//    this->face.registerPrefix(register_prefix2,
//                              (const OnInterestCallback &) bind(&NDNHelper::onInterest, this, _1, _2, _3, _4, _5,
//                                                                false),
//                              bind(&NDNHelper::onRegisterFailed, this, _1));
//    face.setInterestFilter()
//    this->face.processEvents();


//    开始循环处理事件
//    int s = pthread_create(&this->processEventThreadId, NULL, dealEvent, (void *) &face);    //byj
//    if (s != 0) {
//        LOG_ERR("pthread_create: %s\n", strerror(errno));
//        exit(-1);
//    }


    cout << "NDN init success" << endl;
}

/**
 * join thread
 */
void NDNHelper::join() {
    pthread_join(this->processEventThreadId, nullptr);
}

/**
 * 绑定一个LibPcapHelper模块实例
 * @param libPcapHelper
 */
void NDNHelper::bindCacheHelper(CacheHelper* cacheHelper) {
    this->cacheHelper = cacheHelper;
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
        this->rawSocketHelper.sendPacketTo(data.getContent().getBuffer().get(), data.getContent().size(), fileds[3]);
    }
}

/**
 * 内部函数，处理onInterest事件
 * @param prefix
 * @param interest
 * @param face
 * @param isPre 是否是预请求
 */
void NDNHelper::dealOnInterest(const Name &prefix,
                               const Interest &interest, bool isPre) {
    string interest_name = interest.getName().toUri();
//    KeyChain KeyChain_;
//    this->face.setCommandSigningInfo(KeyChain_, KeyChain_.getDefaultCertificateName());
    string pre = "/IP/pre/";
//    if (interest_name.find(pre, 0) != string::npos) {
    if (isPre) {
        string next_name = "/IP";
        vector<string> fileds;
        boost::split(fileds, interest_name, boost::is_any_of("/"));

        string sip = fileds[3];
        string dip = fileds[4];
        string uid = fileds[5];
        next_name.append("/" + dip);
        next_name.append("/" + sip);
        next_name.append("/" + uid);

//        //回复一个空包
//        Data data(interest_name);
//        data.setContent((const uint8_t *) empty_content, sizeof(empty_content));
//        KeyChain_.sign(data);
//        this->face.putData(data);

        //发一个正式拉取的请求
        this->expressInterest(next_name, false);
//        printf("\n================execute empty onInterest================\n");
    } else {
        string uuid = interest_name.substr(28, interest_name.length());
        cout << "处理uuid=" << uuid << endl;
        auto res = cacheHelper->get(uuid);
        if (!res.second) {
            cout << "没有找到uuid = " << uuid << "的数据包" << "(" << interest_name << ")" << endl;
            return;
        } else {
            cout << "成功找到" << endl;
        }
        tuple_t tuple1 = res.first;

        cout << "开始删除" << endl;
        //删除
        cacheHelper->erase(uuid);

        cout << "删除结束" << endl;
        Data data(interest_name);
        data.setContent(tuple1.pkt, tuple1.size);
        KeyChain_.sign(data);
        this->face.put(data);
        cout << "发送数据" << endl;
//        printf("\n================execute onInterest================\n");
    }
}

void NDNHelper::onData(const Interest &interest, const Data &data) {
    cout << "onData: " << interest.getName().toUri() << endl;
    this->dealOnData(data);
}

void NDNHelper::onNack(const Interest &, const lp::Nack &) {

}


void NDNHelper::onTimeout(const Interest &interest, bool isPre) {
    if(!isPre) {
        cout << "Timed out: " << interest.getName().toUri() << endl;
    }
}

//void NDNHelper::onInterest(const Name &prefix,
//                           const Interest &interest, Face &face, uint64_t interestFilterId,
//                           const InterestFilter &filter, bool isPre) {
//    this->dealOnInterest(prefix, interest, face, isPre);
//}

void NDNHelper::onInterest(const Name &prefix, const InterestFilter &filter, const Interest &interest, bool isPre) {
    cout << "onInterest: " << prefix.toUri() << endl;
    this->dealOnInterest(prefix, interest, isPre);
}


void NDNHelper::onRegisterFailed(const Name &prefix) {
    cout << "Register failed for prefix " << prefix.toUri() << endl;
}

void NDNHelper::expressInterest(string name, bool isPre) {
    this->face.expressInterest(Interest(name), bind(&NDNHelper::onData, this, _1, _2),
            bind(&NDNHelper::onNack, this, _1, _2), bind(&NDNHelper::onTimeout, this, _1, isPre));
//    this->face.processEvents();
}

