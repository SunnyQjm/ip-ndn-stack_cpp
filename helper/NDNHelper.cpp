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
    while(true) {
        face.processEvents();
        usleep(50);
    }
}

//all-flow function
void* get_ip_fun(void *arg) {
    tuple_t t;
    memset(&t, 0, sizeof(struct Tuple));
    tuple_t * tail = head = (tuple_t*)malloc(sizeof(tuple_t));
    tail->next = NULL;
    while (true) {
        while (read_ringbuffer(rb_all_flow, &t) < 0) {}; //read the ringbuff
        tail->next = (tuple_t *)malloc(sizeof(tuple_t));
        tail = tail->next ;
        memcpy(head,&t,sizeof(tuple_t));
        //	printf("144 tuple sip:%d ,dip:%d ,flag:%d ,size:%d\n ,uid:%d\n", t.key.src_ip, t.key.dst_ip, t.flag, t.size, t.index);
        uint32_t int_sip = ntohl(t.key.src_ip);
        uint32_t int_dip = ntohl(t.key.dst_ip);
        //cout<<int_sip<<endl;
        string sip1 = to_string((int_sip >> 24) & 0xFF);
        string sip2 = to_string((int_sip >> 16) & 0xFF);
        string sip3 = to_string((int_sip >> 8) & 0xFF);
        string sip4 = to_string((int_sip) & 0xFF);
        string dip1 = to_string((int_dip >> 24) & 0xFF);
        string dip2 = to_string((int_dip >> 16) & 0xFF);
        string dip3 = to_string((int_dip >> 8) & 0xFF);
        string dip4 = to_string((int_dip) & 0xFF);
        string uuid = to_string(t.index);
        string name = "IP/pre/";
        //char * ip = (char*) (&t.key.src_ip);
        name.append(dip1);
        name.append(".");
        name.append(dip2);
        name.append(".");
        name.append(dip3);
        name.append(".");
        name.append(dip4);
        name.append("/");

        name.append(sip1);
        name.append(".");
        name.append(sip2);
        name.append(".");
        name.append(sip3);
        name.append(".");
        name.append(sip4);
        name.append("/");

        name.append(uuid);

        face.expressInterest(name, bind(&Consumer::onData, &consumer, _1, _2), bind(&Consumer::onTimeout, &consumer, _1));

    }
    pthread_exit(NULL);
}

/**
 * 初始化与NDN交互模块
 * @param configFilePath 配置文件的路径
 * @return
 */
pthread_t NDNHelper::initNDN(string configFilePath) {
    JSONCPPHelper jsoncppHelper(configFilePath);
    string registerIp = jsoncppHelper.getString(NDNHelper::KEY_CONFIG_REGISTER_IP);
    cout << "registerIp: " << registerIp << endl;
    // 前缀注册
    KeyChain KeyChain_;
    this->face.setCommandSigningInfo(KeyChain_, KeyChain_.getDefaultCertificateName());
    Name register_prefix1("/IP/192.169.1.1");
    Name register_prefix2("/IP/pre/192.169.1.1");
    this->face.registerPrefix(register_prefix1,(const OnInterestCallback&)bind(&Producer::onInterest,&producer, _1, _2, _3, _4, _5),bind(&Producer::onRegisterFailed,&producer, _1));
    this->face.registerPrefix(register_prefix2,(const OnInterestCallback&)bind(&Producer::onInterest,&producer, _1, _2, _3, _4, _5),bind(&Producer::onRegisterFailed,&producer, _1));

    //开始循环处理事件
    pthread_t threadId;   //byj
    pthread_create(&threadId, NULL, dealEvent, NULL);    //byj

    return threadId;
}

