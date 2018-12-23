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
#include "config.h"
#include <pcap.h>
#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>
using namespace ndn; 
using namespace ndn::func_lib; 
using namespace std;

extern conf_t* conf;
//init variables for ringbuff
ringbuffer_t* rb_all_flow;
pthread_t all_flow_thread;
//transform timeval to double
tuple_t * head = NULL;
char empty_content[]="none";
char content[]="success";

void showpcap(const ptr_lib::shared_ptr<Data>& data);
void sendpcap(const ptr_lib::shared_ptr<const Name>& prefix,const ptr_lib::shared_ptr<const Interest>& interest, Face& face);

class Consumer { 
	public: 
		int callbackCount_; 
		Consumer() { callbackCount_ = 0; } 
		void onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data) { 
			//callbackCount_+=100; 
			showpcap(data);
		} 
		void onTimeout(const ptr_lib::shared_ptr<const Interest>& interest) { 
			//callbackCount_+=100; 
			cout << "Timed out: " << interest->getName().toUri() << endl; 
		} 
};

class Producer {
	public:
		int callbackCount_; 
		Producer() { callbackCount_ = 0; } 
		void onInterest(const ptr_lib::shared_ptr<const Name>& prefix,
				const ptr_lib::shared_ptr<const Interest>& interest, Face& face,
				uint64_t interestFilterId,
				const ptr_lib::shared_ptr<const InterestFilter>& filter) {
			//callbackCount_+=100;
			sendpcap(prefix,interest,face);

		}
		void onRegisterFailed(const ptr_lib::shared_ptr<const Name>& prefix)
		{
			//callbackCount_+=100; 
			cout << "Register failed for prefix " << prefix->toUri() << endl;
		}

};

Producer producer;
Consumer consumer;
Face face("localhost");

void sendpcap(const ptr_lib::shared_ptr<const Name>& prefix,const ptr_lib::shared_ptr<const Interest>& interest, Face& face) {
     string interest_name = interest->getName().toUri();
     KeyChain KeyChain_;
     string pre = "/IP/pre/";
     if(interest_name.find (pre,0) != string::npos){
         string next_name = "/IP";
		 int find_index1 = 7 ;
		 int find_index2 = 0 ;
		 find_index1 = interest_name.find('/',find_index1+1);
		 find_index2 = interest_name.find('/',find_index1+1);
         next_name.append(interest_name.substr(find_index1,find_index2-find_index1));
         next_name.append(interest_name.substr(7,find_index1));
         next_name.append(interest_name.substr(find_index2,interest_name.length()-find_index2));

         Data data(next_name);
         data.setContent((const uint8_t*)empty_content,sizeof(empty_content));
         KeyChain_.sign(data);
         face.putData(data);
         face.expressInterest(next_name,bind(&Consumer::onData, &consumer, _1, _2), bind(&Consumer::onTimeout, &consumer, _1));
         printf("\n================execute empty onInterest================\n");
     }
     else{
         string next_name = "/IP";
		 int find_index1 = 3 ;
		 int find_index2 = 0 ;
		 find_index1 = interest_name.find('/',find_index1+1);
		 find_index2 = interest_name.find('/',find_index1+1);
         next_name.append(interest_name.substr(find_index1,find_index2-find_index1));
         next_name.append(interest_name.substr(3,find_index1));
         next_name.append(interest_name.substr(find_index2,interest_name.length()-find_index2));

         unsigned int uuid = atoi(interest_name.substr(28,interest_name.length()).c_str());
         cout << "reply interest :" << next_name << ", " << uuid << endl;
		 while(head->next != NULL){
			 if(uuid == head->next->index)
			 {
				 Data data(next_name);
				 data.setContent((const uint8_t*)head->next->pkt,sizeof(head->next->pkt));
				 KeyChain_.sign(data);
				 face.putData(data);
				 tuple_t* tmp = head->next->next;
				 free(head->next);
				 head->next = tmp ;
				 break ;
			 }
			 tuple_t* tmp = head->next->next;
			 free(head->next);
			 head->next = tmp ;
		 }
		 printf("\n================execute onInterest================\n");
	 }
}

void showpcap(const ptr_lib::shared_ptr<Data>& data) {
	cout<< "Name: " << data->getName().toUri() << endl;
	cout<< "Content: " << data->getContent() << endl;
}

#endif //IP_NDN_STACK_CPP_NDN_H

