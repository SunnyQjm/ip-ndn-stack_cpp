#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class RawSocketHelper {

public:
    RawSocketHelper() {
        //创建一个原始套接字
        this->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

        //开启IP_HDRINCL选项，允许IP首部自定义
        const int on = 1;
        if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
            std::cout << "开启IP_HDRINCL选项" << std::endl;
            exit(-1);
        }
    }


    /**
     * 没有调用connect绑定目的地址的话，需要用sendTo来发送RawSocket包。
     * @param buffer    要发送的IP包，包括包头
     * @param len       整个IP包的长度
     * @param ip        目的IP => 点分十进制格式的ip字符串 eg. 192.168.1.1
     * @param flag      调用方式标志位, 默认为0, 改变Flags，将会改变Sendto发送的形式
     * @return
     */
    ssize_t sendPacketTo(const void * buffer, size_t len, char *ip, int flag = 0) {
        sockaddr_in address{};
        bzero(&address, sizeof(address));
        address.sin_family = AF_INET;
        inet_pton(AF_INET, ip, (void *)&address);
        return sendto(this->sockfd, buffer, len, flag, (sockaddr *)&address, sizeof(address));
    }

private:
    int sockfd;     //原始套接字handler

};

