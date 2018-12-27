#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "tuple.h"
#include "ttime.h"
#include "util.h"

#ifndef __PACKET_H__
#define __PACKET_H__

#define MAX_CAPLEN 1500
#define ETH_LEN 14
#define IP_CHECK 1

enum PACKET_STATUS {
    STATUS_INIT = 0,  //initial state
    STATUS_VALID, //valid packets
    //error status
    STATUS_NON_IP, //not an IP packet
    STATUS_IP_NOT_FULL,  //IP not full packets
    STATUS_IP_VER_FAIL, //IP version failed
    STATUS_IP_CHKSUM_FAIL, //ip CHECKSUM failed
    STATUS_IP_FRAG, // IP fragments
    STATUS_TCP_NOT_FULL, // TCP header not full
    STATUS_UDP_NOT_FULL, // UDP header not full
    STATUS_ICMP_NOT_FULL, // ICMP header not full
    STATUS_UNDEFINED, // (not used for now)
    STATUS_NON_GTP, // not a valid GTP packet
    STATUS_NON_GPRS // not a valid GPRS packet
};



void print_tuple(FILE* f, tuple_t* t);
void read_tuple(char*, tuple_t* t);

void report_final_stat();
void report_final_stat_file(const char* filename);
void reset_stat();
void print_tuple_screen(tuple_t* t);

enum PACKET_STATUS decode(const uint8_t* pkt, uint32_t cap_len, uint32_t act_len, double ts, tuple_p t);


void dbl2time(double time,struct timeval * time_new);

#endif
