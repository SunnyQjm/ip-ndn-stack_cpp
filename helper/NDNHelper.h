//
// Created by mingj on 18-12-22.
//

#ifndef IP_NDN_STACK_CPP_NDNHELPER_H
#define IP_NDN_STACK_CPP_NDNHELPER_H

#include <iostream>
#include "../libpcapcapture/ndn.cpp"
#include "JSONCPPHelper.h"

using namespace std;
class NDNHelper {
public:
    NDNHelper();
    pthread_t initNDN(string configFilePath);

public: //静态变量
    static const string PREFIX_PRE_REQUEST;
    static const string PREFIX_REQUEST_DATA;
    static const string KEY_CONFIG_REGISTER_IP;
private:
    Face face;
};

//前缀
const string NDNHelper::PREFIX_PRE_REQUEST = "/IP/pre";
const string NDNHelper::PREFIX_REQUEST_DATA = "/IP";
//配置文件的键值
const string NDNHelper::KEY_CONFIG_REGISTER_IP = "registerIp";

#endif //IP_NDN_STACK_CPP_NDNHELPER_H
