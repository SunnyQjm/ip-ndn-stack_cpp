//
// Created by mingj on 18-12-22.
//

#include "NDNHelper.h"


NDNHelper::NDNHelper(): face("localhost") {
}

/**
 * 在子线程中处理循环处理事件
 * @param arg
 * @return
 */
void *dealEvent(void* arg) {
    cout << "dealEvent" << endl;
    while(true) {
        face.processEvents();
        usleep(50);
    }
}



/**
 * 初始化与NDN交互模块
 * @param configFilePath 配置文件的路径
 * @return
 */
void NDNHelper::initNDN(string configFilePath) {
    JSONCPPHelper jsoncppHelper(configFilePath);
    string registerIp = jsoncppHelper.getString(NDNHelper::KEY_CONFIG_REGISTER_IP);
    cout << "registerIp: " << registerIp << endl;
    // 前缀注册
    KeyChain KeyChain_;
    this->face.setCommandSigningInfo(KeyChain_, KeyChain_.getDefaultCertificateName());
    Name register_prefix1(NDNHelper::PREFIX_PRE_REQUEST + "/" + registerIp);
    Name register_prefix2(NDNHelper::PREFIX_REQUEST_DATA + "/" + registerIp);
    this->face.registerPrefix(register_prefix1,(const OnInterestCallback&)bind(&Producer::onInterest,&producer, _1, _2, _3, _4, _5),bind(&Producer::onRegisterFailed,&producer, _1));
    this->face.registerPrefix(register_prefix2,(const OnInterestCallback&)bind(&Producer::onInterest,&producer, _1, _2, _3, _4, _5),bind(&Producer::onRegisterFailed,&producer, _1));

    //开始循环处理事件
    int s = pthread_create(&this->processEventThreadId, NULL, dealEvent, NULL);    //byj
    if(s != 0) {
        LOG_ERR("pthread_create: %s\n", strerror(errno));
        exit(-1);
    }


//    cout << "NDN init success" << endl;
}

/**
 * join thread
 */
void NDNHelper::join() {
    pthread_join(this->processEventThreadId, nullptr);
}

