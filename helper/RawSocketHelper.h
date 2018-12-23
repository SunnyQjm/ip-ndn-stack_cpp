//
// Created by mingj on 18-12-22.
//

#ifndef IP_NDN_STACK_CPP_RAWSOCKETHELPER_H
#define IP_NDN_STACK_CPP_RAWSOCKETHELPER_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
class RawSocketHelper {

public:
    RawSocketHelper();

    /**
     * 没有调用connect绑定目的地址的话，需要用sendTo来发送RawSocket包。
     * @param buffer    要发送的IP包，包括包头
     * @param len       整个IP包的长度
     * @param ip        目的IP => 点分十进制格式的ip字符串 eg. 192.168.1.1
     * @param flag      调用方式标志位, 默认为0, 改变Flags，将会改变Sendto发送的形式
     * @return
     */
    ssize_t sendPacketTo(const void * buffer, size_t len, string &ip, int flag = 0);

private:
    int sockfd;     //原始套接字handler

};

#endif //IP_NDN_STACK_CPP_RAWSOCKETHELPER_H
