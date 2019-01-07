#include "packet.h"
char ip1[30]; char ip2[30];

/* Convert IP (in network order) to String */
static inline char* ip2a(uint32_t ip, char* addr) {
	sprintf(addr, "%d.%d.%d.%d", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, (ip >> 24) & 0xff);
	return addr;
}
struct PacketStat {
	uint64_t tot_pkt_cnt; // total # of packets observed 
	uint64_t tot_cap_byte_cnt; //total # of captured bytes observed
	uint64_t tot_act_byte_cnt; //total # of actual bytes observed 
	uint64_t valid_pkt_cnt; //# of valid packets
	uint64_t valid_cap_byte_cnt; //# of valid packets
	uint64_t valid_act_byte_cnt; //# of valid packets
	uint64_t non_ip_cnt; // # of non-IP packets
	uint64_t ip_not_full_cnt; //# of IP non-full packets
	uint64_t ip_ver_fail_cnt;  //# of IP version failed packets
	uint64_t ip_chksum_fail_cnt; //# of IP checksum failed packets
	uint64_t ip_frag_cnt; //# of IP fragments
	uint64_t tcp_not_full_cnt; //# of TCP-not-full packets
	uint64_t udp_not_full_cnt; //# of UDP-not-full packets
	uint64_t icmp_not_full_cnt; //# of ICMP-not-full packets
	uint64_t undefined_cnt;
	uint64_t non_gtp_cnt;
	uint64_t non_gprs_cnt;
	uint64_t used_time;
	double trace_start_ts;
	double trace_end_ts;
}packet_stat;

uint64_t seq_count;

void dbl2time(double time,struct timeval * time_new)
{
	int temp_time1=(int) time;
	//printf("temp_time1:%d **** ",temp_time1);
	double temp_time2=time-(double)temp_time1;
	//printf("temp_time2:%lf\n",temp_time2);
	time_new->tv_sec=temp_time1;
	time_new->tv_usec=temp_time2*1000.0*1000.0;
	//printf("\ntv_usec:%d\n",time_new->tv_usec);
}


void report_final_stat_file(const char* filename) {
    printf("%.2lf Gb/s\n", packet_stat.tot_act_byte_cnt*8.0 / packet_stat.used_time * 1.0e6 / GB); // packet handle speed
    
    FILE* output = fopen(filename, "w");

    if(output == NULL) {
        fprintf(stderr, "\nRunning Stat\n"
            "=========================================\n"
            "\tUsed time: %.2lf s\n"
            "\tSpeed: %.2lf packets/s (%.2lf Gb/s)\n"
            "\tTrace duration: %.2lf s\n",
            packet_stat.used_time / 1.0e6,
            packet_stat.tot_pkt_cnt*1.0 / packet_stat.used_time * 1.0e6,
            packet_stat.tot_act_byte_cnt*8.0 / packet_stat.used_time * 1.0e6 / GB,
            packet_stat.trace_end_ts-packet_stat.trace_start_ts);

        fprintf(stderr, "\nPacket Stat\n"
            "=========================================\n"
            "\tTotal packets observed: %lu\n"
            "\tTotal bytes observed (capture): %lu\n"
            "\tTotal bytes observed (actual): %lu\n"
            "\tValid packet count: %lu\n"
            "\tValid byte count(capture): %lu\n"
            "\tValid byte count(actual): %lu\n"
            "\tNon-IP packet count: %lu\n"
            "\tIP-not-full packet count: %lu\n"
            "\tIP-version-failed packet count: %lu\n"
            "\tIP-checksum-failed packet count: %lu\n"
            "\tIP-fragment packet count: %lu\n"
            "\tTCP-not-full packet count: %lu\n"
            "\tUDP-not-full packet count: %lu\n"
            "\tICMP-not-full packet count: %lu\n"
            "\tUndefined packet count: %lu\n"
            "\tNon-GTP packet count: %lu\n"
            "\tNon-GPRS packet count: %lu\n"
            "=========================================\n",
            packet_stat.tot_pkt_cnt,
            packet_stat.tot_cap_byte_cnt,
            packet_stat.tot_act_byte_cnt,
            packet_stat.valid_pkt_cnt,
            packet_stat.valid_cap_byte_cnt,
            packet_stat.valid_act_byte_cnt,
            packet_stat.non_ip_cnt,
            packet_stat.ip_not_full_cnt,
            packet_stat.ip_ver_fail_cnt,
            packet_stat.ip_chksum_fail_cnt++,
            packet_stat.ip_frag_cnt++,
            packet_stat.tcp_not_full_cnt,
            packet_stat.udp_not_full_cnt,
            packet_stat.icmp_not_full_cnt,
            packet_stat.undefined_cnt,
            packet_stat.non_gtp_cnt,
            packet_stat.non_gprs_cnt);
    }
    else {
        fprintf(output, "\nRunning Stat\n"
            "=========================================\n"
            "\tUsed time: %.2lf s\n"
            "\tSpeed: %.2lf packets/s (%.2lf Gb/s)\n"
            "\tTrace duration: %.2lf s\n",
            packet_stat.used_time / 1.0e6,
            packet_stat.tot_pkt_cnt*1.0 / packet_stat.used_time * 1.0e6,
            packet_stat.tot_act_byte_cnt*8.0 / packet_stat.used_time * 1.0e6 / GB,
            packet_stat.trace_end_ts-packet_stat.trace_start_ts);

        fprintf(output, "\nPacket Stat\n"
            "=========================================\n"
            "\tTotal packets observed: %lu\n"
            "\tTotal bytes observed (capture): %lu\n"
            "\tTotal bytes observed (actual): %lu\n"
            "\tValid packet count: %lu\n"
            "\tValid byte count(capture): %lu\n"
            "\tValid byte count(actual): %lu\n"
            "\tNon-IP packet count: %lu\n"
            "\tIP-not-full packet count: %lu\n"
            "\tIP-version-failed packet count: %lu\n"
            "\tIP-checksum-failed packet count: %lu\n"
            "\tIP-fragment packet count: %lu\n"
            "\tTCP-not-full packet count: %lu\n"
            "\tUDP-not-full packet count: %lu\n"
            "\tICMP-not-full packet count: %lu\n"
            "\tUndefined packet count: %lu\n"
            "\tNon-GTP packet count: %lu\n"
            "\tNon-GPRS packet count: %lu\n"
            "=========================================\n",
            packet_stat.tot_pkt_cnt,
            packet_stat.tot_cap_byte_cnt,
            packet_stat.tot_act_byte_cnt,
            packet_stat.valid_pkt_cnt,
            packet_stat.valid_cap_byte_cnt,
            packet_stat.valid_act_byte_cnt,
            packet_stat.non_ip_cnt,
            packet_stat.ip_not_full_cnt,
            packet_stat.ip_ver_fail_cnt,
            packet_stat.ip_chksum_fail_cnt++,
            packet_stat.ip_frag_cnt++,
            packet_stat.tcp_not_full_cnt,
            packet_stat.udp_not_full_cnt,
            packet_stat.icmp_not_full_cnt,
            packet_stat.undefined_cnt,
            packet_stat.non_gtp_cnt,
            packet_stat.non_gprs_cnt);
        fclose(output);
    }
}

void report_final_stat() {
    report_final_stat_file(NULL);
}

/* IP Header checksum
 * @param w - short words of IP header
 * @param len - header length (in bytes)
 * @return - 0 if the answer is correct, a non-zero value if there's error
**/
inline static unsigned short in_chksum_ip(unsigned short* w, int len) 
{
    long sum = 0;
    
    while (len > 1) {
        sum += *w++;
        if (sum & 0x80000000) /* if high order bit sen, fold */
            sum = (sum & 0xFFFF) + (sum >> 16);
        len -= 2;
    }  

    if (len) 
        sum += *w;

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    
    return ~sum;
}

enum PACKET_STATUS decode(const uint8_t* pkt,
        uint32_t cap_len,
        uint32_t act_len,
        double ts,
        tuple_p p) {
    struct ether_header* eth_hdr;
    struct ip* ip_hdr;
    struct tcphdr* tcp_hdr;
    struct udphdr* udp_hdr;
    int eth_len = ETH_LEN;
    enum PACKET_STATUS status;
	//char* date;
	uint8_t* date;
    p->ipSize = cap_len - ETH_LEN;

    status = STATUS_VALID; //valid packets
    packet_stat.tot_pkt_cnt++;
    if (packet_stat.tot_pkt_cnt == 1) {
        packet_stat.trace_start_ts = ts;
    }
    if (ts < packet_stat.trace_end_ts) {
        LOG_WARN("Skewed ts\n");
    }
    packet_stat.trace_end_ts = ts;
    packet_stat.tot_cap_byte_cnt += cap_len;
    packet_stat.tot_act_byte_cnt += act_len;

    //error checking(Ethernet level)
    if (eth_len == 14) {
        eth_hdr = (struct ether_header*)pkt;
        if (ntohs(eth_hdr->ether_type) == ETHERTYPE_VLAN) {
            eth_len = 18;
        }
        else if (ntohs(eth_hdr->ether_type) != ETH_P_IP) {
            status = STATUS_NON_IP;
        }
    }
    else if (eth_len == 4) {
        if (ntohs(*(uint16_t*)(pkt + 2)) != ETH_P_IP) {
            status = STATUS_NON_IP;
        }
    }
    else if (eth_len != 0) {
        // unkown ethernet header length
        status = STATUS_NON_IP;
    }

    uint32_t len = cap_len - eth_len;

    //error checking(IP level)
    ip_hdr = (struct ip*)(pkt + eth_len);

    p->flag = *((uint16_t *)(pkt + 48));

    date = (uint8_t*)(pkt + eth_len);
    // i) IP header length check
    // LOG_MSG("check 1\n");
    if ((int)len < (ip_hdr->ip_hl << 2)) {
        status = STATUS_IP_NOT_FULL;
    }
    // ii) IP version check
    // LOG_MSG("check 2\n");
    if (ip_hdr->ip_v != 4) {
        status = STATUS_IP_VER_FAIL;
    }
    // iii) IP checksum check
    // LOG_MSG("check 3\n");
    if (IP_CHECK && in_chksum_ip((unsigned short*)ip_hdr, ip_hdr->ip_hl << 2)) {
        status = STATUS_IP_CHKSUM_FAIL;
    }

    // LOG_MSG("check 4\n");
    // error checking (TCP\UDP\ICMP layer test)
    if (ip_hdr->ip_p == IPPROTO_TCP) {
        //see if the TCP header is fully captured
        tcp_hdr = (struct tcphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        if ((int)len < (ip_hdr->ip_hl << 2) + (tcp_hdr->doff <<2)) {
            status = STATUS_TCP_NOT_FULL;
        }
    }
    else if (ip_hdr->ip_p == IPPROTO_UDP) {
        //see if the UDP header is fully captured
        if ((int)len < (ip_hdr->ip_hl << 2) + 8) {
            status = STATUS_UDP_NOT_FULL;
        }
    }
    else if (ip_hdr->ip_p == IPPROTO_ICMP) {
        //see if the ICMP header is fully captured
        if ((int)len < (ip_hdr->ip_hl << 2) + 8) {
            status = STATUS_ICMP_NOT_FULL;
        }
    }
	//date = (char*)((uint8_t*)tcp_hdr + (tcp_hdr->doff << 2))��
    switch (status) {
        case STATUS_VALID:
            packet_stat.valid_pkt_cnt++;
            packet_stat.valid_cap_byte_cnt += cap_len;
            packet_stat.valid_act_byte_cnt += act_len;
            break;
        case STATUS_NON_IP:
            packet_stat.non_ip_cnt++;
            LOG_DEBUG("non valid status: non ip\n");
            break;
        case STATUS_IP_NOT_FULL:
            packet_stat.ip_not_full_cnt++;
            LOG_DEBUG("non valid status: ip not full\n");
            break;
        case STATUS_IP_VER_FAIL:
            packet_stat.ip_ver_fail_cnt++;
            LOG_DEBUG("non valid status: ip ver fail\n");
            break;
        case STATUS_IP_CHKSUM_FAIL:
            packet_stat.ip_chksum_fail_cnt++;
            LOG_DEBUG("non valid status: ip chksum fail\n");
            break;
        case STATUS_IP_FRAG:
            packet_stat.ip_frag_cnt++;
            LOG_DEBUG("non valid status: ip frag\n");
            break;
        case STATUS_TCP_NOT_FULL:
            packet_stat.tcp_not_full_cnt++;
            LOG_DEBUG("non valid status: tcp not full\n");
            break;
        case STATUS_UDP_NOT_FULL:
            packet_stat.udp_not_full_cnt++;
            LOG_DEBUG("non valid status: udp not full\n");
            break;
        case STATUS_ICMP_NOT_FULL:
            packet_stat.icmp_not_full_cnt++;
            LOG_DEBUG("non valid status: icmp not full\n");
            break;
        case STATUS_UNDEFINED:
            packet_stat.undefined_cnt++;
            LOG_DEBUG("non valid status: pakcet not defined\n");
            break;
        case STATUS_NON_GTP:
            packet_stat.non_gtp_cnt++;
            LOG_DEBUG("non valid status: non gtp\n");
            break;
        case STATUS_NON_GPRS:
            packet_stat.non_gprs_cnt++;
            LOG_DEBUG("non valid status: non gprs\n");
            break;
        default:
            break;
    }
    if (status != STATUS_VALID) 
        return status;
    
    p->seq = ++seq_count;

    //assign the fields
    p->key.src_ip = ip_hdr->ip_src.s_addr;
    p->key.dst_ip = ip_hdr->ip_dst.s_addr;
    p->key.proto = ip_hdr->ip_p;
    p->pkt_ts = ts;
    p->size = ntohs(ip_hdr->ip_len);
	//all paket
	p->pkt = date;

    if (ip_hdr->ip_p == IPPROTO_TCP) {
        //TCP
        tcp_hdr = (struct tcphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        p->key.src_port = ntohs(tcp_hdr->source);
        p->key.dst_port = ntohs(tcp_hdr->dest);
    }
    else if (ip_hdr->ip_p == IPPROTO_UDP) {
        //UDP
        udp_hdr = (struct udphdr*)((uint8_t*)ip_hdr + (ip_hdr->ip_hl << 2));
        p->key.src_port = ntohs(udp_hdr->source);
        p->key.dst_port = ntohs(udp_hdr->dest);
    } else {
        // Other L4
        p->key.src_port = 0;
        p->key.dst_port = 0;
    }
    return status;
}

void print_tuple(FILE* f, tuple_t* t) {
    char ip1[30], ip2[30];

    fprintf(f, "%s(%u) <-> %s(%u) %u %ld\n",
            ip2a(t->key.src_ip, ip1), t->key.src_port,
            ip2a(t->key.dst_ip, ip2), t->key.dst_port,
            t->key.proto, t->size);
}

void print_tuple_screen(tuple_t* t)
{
//    char ip1[30], ip2[30];
//
//    printf("source ip and port:%s(%u) <->destination ip and port: %s(%u) %u %ld\n",
//            ip2a(t->key.src_ip, ip1), t->key.src_port,
//            ip2a(t->key.dst_ip, ip2), t->key.dst_port,
//            t->key.proto, t->size);
//    formal_time_t temp_time1;
//    struct timeval temp_time2;
//    double time=t->pkt_ts;
//    dbl2time(time,&temp_time2);
////    time_val_to_Time(temp_time2,&temp_time1);
//    printf(" time stamp: year.%d month.%d day.%d hour.%d minute.%d sec.%d m_sec.%d\n",temp_time1.year,temp_time1.month,temp_time1.day,temp_time1.hour,temp_time1.minute,temp_time1.sec,temp_time1.m_sec);
}

void read_tuple(char* line, tuple_t* p) {
    unsigned int ip1, ip2, ip3, ip4, ip5, ip6, ip7, ip8;
    sscanf(line, "%u.%u.%u.%u(%hu) <-> %u.%u.%u.%u(%hu) %hhu %lu",
        &ip1, &ip2, &ip3, &ip4,
        &p->key.src_port,
        &ip5, &ip6, &ip7, &ip8,
        &p->key.dst_port,
        &p->key.proto,
        &p->size);
    p->key.src_ip = (ip4 << 24) | (ip3 << 16) | (ip2 << 8) | ip1;
    p->key.dst_ip = (ip8 << 24) | (ip7 << 16) | (ip6 << 8) | ip5;
}

void reset_stat() {
    memset(&packet_stat, 0, sizeof(struct PacketStat));
}
