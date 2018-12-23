#ifndef IP_NDN_STACK_CPP_NDN_H
#define IP_NDN_STACK_CPP_NDN_H

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <math.h>
#include <semaphore.h>
#include "util.h"
#include "ringbuffer.h"
#include "packet.h"
#include "hash.h"
//#include "config.h"
#include <pcap.h>
#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "../helper/RawSocketHelper.h"

using namespace ndn;
using namespace ndn::func_lib;
using namespace std;

//extern conf_t *conf;
//init variables for ringbuff
ringbuffer_t *rb_all_flow;
pthread_t all_flow_thread;
//transform timeval to double
//tuple_t *head = NULL;
unordered_map<string, tuple_t> ipPacketCache;
char empty_content[] = "none";
char content[] = "success";
RawSocketHelper rawSocketHelper;

void showpcap(const ptr_lib::shared_ptr<Data> &data);

void sendpcap(const ptr_lib::shared_ptr<const Name> &prefix, const ptr_lib::shared_ptr<const Interest> &interest,
              Face &face);

class Consumer {
public:
    int callbackCount_;

    Consumer() { callbackCount_ = 0; }

    void onData(const ptr_lib::shared_ptr<const Interest> &interest, const ptr_lib::shared_ptr<Data> &data) {
        //callbackCount_+=100;
        showpcap(data);
    }

    void onTimeout(const ptr_lib::shared_ptr<const Interest> &interest) {
        //callbackCount_+=100;
        cout << "Timed out: " << interest->getName().toUri() << endl;
    }
};

class Producer {
public:
    int callbackCount_;

    Producer() { callbackCount_ = 0; }

    void onInterest(const ptr_lib::shared_ptr<const Name> &prefix,
                    const ptr_lib::shared_ptr<const Interest> &interest, Face &face,
                    uint64_t interestFilterId,
                    const ptr_lib::shared_ptr<const InterestFilter> &filter) {
        cout << "onInterest" << endl;
        //callbackCount_+=100;
        sendpcap(prefix, interest, face);

    }

    void onRegisterFailed(const ptr_lib::shared_ptr<const Name> &prefix) {
        //callbackCount_+=100;
        cout << "Register failed for prefix " << prefix->toUri() << endl;
    }

};

Producer producer;
Consumer consumer;

void sendpcap(const ptr_lib::shared_ptr<const Name> &prefix, const ptr_lib::shared_ptr<const Interest> &interest,
              Face &face) {
    string interest_name = interest->getName().toUri();
    KeyChain KeyChain_;
    string pre = "/IP/pre/";
    if (interest_name.find(pre, 0) != string::npos) {
        string next_name = "/IP";
        vector<string> fileds;
        boost::split(fileds, interest_name, boost::is_any_of("/"));

        string sip = fileds[3];
        string dip = fileds[4];
        string uid = fileds[5];
        next_name.append("/" + dip);
        next_name.append("/" + sip);
        next_name.append("/" + uid);

        //回复一个空包
        Data data(interest_name);
        data.setContent((const uint8_t *) empty_content, sizeof(empty_content));
        KeyChain_.sign(data);
        face.putData(data);

        //发一个正式拉取的请求
        face.expressInterest(next_name, bind(&Consumer::onData, &consumer, _1, _2),
                             bind(&Consumer::onTimeout, &consumer, _1));
        printf("\n================execute empty onInterest================\n");
    } else {
//        int find_index1 = 3;
//        int find_index2 = 0;
//        find_index1 = interest_name.find('/', find_index1 + 1);
//        find_index2 = interest_name.find('/', find_index1 + 1);
//        next_name.append(interest_name.substr(find_index1, find_index2 - find_index1));
//        next_name.append(interest_name.substr(3, find_index1));
//        next_name.append(interest_name.substr(find_index2, interest_name.length() - find_index2));

//        unsigned int uuid = atoi(interest_name.substr(28, interest_name.length()).c_str());
        string uuid = interest_name.substr(28, interest_name.length());
        cout << "reply interest :" << interest_name << ", " << uuid << endl;
        auto result = ipPacketCache.find(uuid);
        if(result == ipPacketCache.end()) {
            cout << "没有找到uuid = " << uuid << "的数据包" << endl;
            return;
        }
        tuple_t tuple1 = result->second;

        Data data(interest_name);
        data.setContent(tuple1.pkt, tuple1.size);
        KeyChain_.sign(data);
        face.putData(data);

//        while (head->next != NULL) {
//            cout << "finding" << endl;
//            if (uuid == head->next->index) {
//                Data data(next_name);
//                data.setContent((const uint8_t *) head->next->pkt, sizeof(head->next->pkt));
//                KeyChain_.sign(data);
//                face.putData(data);
//                tuple_t *tmp = head->next->next;
//                free(head->next);
//                head->next = tmp;
//                break;
//            }
//            tuple_t *tmp = head->next->next;
//            free(head->next);
//            head->next = tmp;
//        }
        printf("\n================execute onInterest================\n");
    }
}

void showpcap(const ptr_lib::shared_ptr<Data> &data) {
    string name = data->getName().toUri();
    cout << "Name: " << data->getName().toUri() << endl;
    cout << "Content: " << data->getContent() << endl;
    string pre = "/IP/pre/";
    if (name.find(pre, 0) != string::npos) {

    } else {        //正式拉取到包的回复
        vector<string> fileds;
        boost::split(fileds, name, boost::is_any_of("/"));

        cout << "发raw socket 到: " << fileds[3] << endl;
        rawSocketHelper.sendPacketTo(data->getContent().buf(), data->getContent().size(), fileds[3]);
    }
}

#endif //IP_NDN_STACK_CPP_NDN_H

