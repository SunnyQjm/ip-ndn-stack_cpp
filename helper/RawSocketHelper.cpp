#include "RawSocketHelper.h"

RawSocketHelper::RawSocketHelper() {
    //创建一个原始套接字
    this->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    //开启IP_HDRINCL选项，允许IP首部自定义
    const int on = 1;
    if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        exit(-1);
    }
}

ssize_t RawSocketHelper::sendPacketTo(const void *buffer, size_t len, string &ip, int flag) {
    sockaddr_in address{};
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    return sendto(this->sockfd, buffer, len, flag, (sockaddr *)&address, sizeof(address));
}
