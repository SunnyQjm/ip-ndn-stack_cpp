//
// Created by mingj on 18-12-23.
//

#include "LibPcapHelper.h"

double time2dbl(struct timeval time_value) {
    double new_time = 0;
    new_time = (double)(time_value.tv_usec);
    new_time /= 1000000;
    new_time += (double)time_value.tv_sec;
    return(new_time);

}


LibPcapHelper::LibPcapHelper(): face("localhost") {

}

void LibPcapHelper::initLibPcap(string configFilePath) {
    JSONCPPHelper jsoncppHelper(configFilePath);
    //    interval_len = conf_common_interval_len(conf);  //the value of interva    l_len is the "upload period" in config.ini, in ms
    char tmp[1024];
    string dev_name = jsoncppHelper.getString("pcap_if");

    //const char *dev_name = pcap_lookupdev(NULL);//the value of dev_name is     the interface used for packet capturing in config.ini
    struct pcap_pkthdr *header;
    const uint8_t *pkt = nullptr;
    char ebuf[PCAP_ERRBUF_SIZE];

    int bufsize = jsoncppHelper.getInt("pcap_buf_size"); //the value of bufsize is     the the capturing buffer size in config.ini
    int snaplen = jsoncppHelper.getInt("pcap_snap_len"); //the value of snaplen is     the snapshot length for pcap handler in config.ini
    int to_ms = jsoncppHelper.getInt("pcap_to_ms"); //the value of to_ms is the read timeout for pcap handler, in ms
    //init ringbuffer (there are two ringbuffers)
    sprintf(tmp, "pub1%02d", 0); //fomat the tmp characters.
    tuple_t t_kern;
    double pkt_ts;
    memset(&t_kern, 0, sizeof(struct Tuple));

    int res;
    //init pcap
    this->ph = pcap_create(dev_name.c_str(), ebuf); //dev_name is the network device to open, ebuf is error buffer
    if (ph == nullptr) {
        pcap_close(ph);
        cout << "ph create failed" << endl;
        // printf("%s: pcap_create failed: %s\n", dev_name, ebuf);
        exit(-1);
    }
    if (pcap_set_snaplen(ph, snaplen) || pcap_set_buffer_size(ph, bufsize) ||
        pcap_set_timeout(ph, to_ms) || pcap_set_immediate_mode(ph, 1) ||
        pcap_activate(ph)) { //activate the handle, start to capture
        pcap_close(ph);
        exit(-1);
    }
    bpf_u_int32 net;
    bpf_u_int32 mask;
    pcap_lookupnet(dev_name.c_str(), &net, &mask, ebuf);
    string filter_app = jsoncppHelper.getString("pcap_dstmac");
    //capture packets and copy the packets to the ringbuffer
    while ((res = pcap_next_ex(ph, &header, &pkt)) >= 0) { //reads the next packe    t and returns a success/failure indication.
        if (pkt == nullptr || res == 0) {
                continue;
        }
        //decode the captured packet
        //char filter_app[] = "ether dst 00:1e:67:83:0c:0a";
        struct bpf_program filter{};
        pcap_compile(ph, &filter, filter_app.c_str(), 0, net);
        pcap_setfilter(ph, &filter);
        pkt_ts = time2dbl(header->ts); //doubleֵ
        //
        decode(pkt, header->caplen, header->len, pkt_ts, &t_kern);
        this->deal(t_kern);
    }

}

void LibPcapHelper::close() {
    //free the pcap handler
    pcap_close(this->ph);
}

void LibPcapHelper::join() {
//    pthread_join(this->readRingBufferThreadId, nullptr);
}

void LibPcapHelper::deal(tuple_t tuple) {
    string uuid = this->generateUUID();
    auto result = ipPacketCache.insert(make_pair(uuid, tuple));
    if(!result.second) {
        cout << "插入失败" << endl;
    }
    auto res = ipPacketCache.find(uuid);
    if(res == ipPacketCache.end()) {
        cout << "没有找到uuid = " << uuid << "的数据包" << endl;
        return;
    }

    //发送兴趣包
    uint32_t int_sip = ntohl(tuple.key.src_ip);
    uint32_t int_dip = ntohl(tuple.key.dst_ip);
    //cout<<int_sip<<endl;
    string sip1 = to_string((int_sip >> 24) & 0xFF);
    string sip2 = to_string((int_sip >> 16) & 0xFF);
    string sip3 = to_string((int_sip >> 8) & 0xFF);
    string sip4 = to_string((int_sip) & 0xFF);
    string dip1 = to_string((int_dip >> 24) & 0xFF);
    string dip2 = to_string((int_dip >> 16) & 0xFF);
    string dip3 = to_string((int_dip >> 8) & 0xFF);
    string dip4 = to_string((int_dip) & 0xFF);

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

string LibPcapHelper::generateUUID() {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); // 这里是两个() ，因为这里是调用的 () 的运算符重载
    const string tmp_uuid = boost::uuids::to_string(a_uuid);
//    cout << "generate uuid: " << tmp_uuid << endl;
    return tmp_uuid;
}
