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

LibPcapHelper::LibPcapHelper() {

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
    rb_all_flow = create_ringbuffer_shm1(tmp, sizeof(tuple_t));  //create a ringbuffer
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
        // printf("Capturing %s failed: %s\n", dev_name,
        //	 pcap_geterr(ph));
        pcap_close(ph);
        exit(-1);
    }
    bpf_u_int32 net;
    bpf_u_int32 mask;
    pcap_lookupnet(dev_name.c_str(), &net, &mask, ebuf);
    //capture packets and copy the packets to the ringbuffer
    while ((res = pcap_next_ex(ph, &header, &pkt)) >= 0) { //reads the next packe    t and returns a success/failure indication.
        if (pkt == nullptr || res == 0) {
                continue;
        }
        //decode the captured packet
        const char *filter_app = jsoncppHelper.getString("pcap_dstmac").c_str();
        //char filter_app[] = "ether dst 00:1e:67:83:0c:0a";
        struct bpf_program filter{};
        pcap_compile(ph, &filter, filter_app, 0, net);
        pcap_setfilter(ph, &filter);
        pkt_ts = time2dbl(header->ts); //doubleֵ
        //
        decode(pkt, header->caplen, header->len, pkt_ts, &t_kern);
        cout << header->caplen << "-" << header->len << endl;
        t_kern.index = AwareHash((uint8_t*)t_kern.pkt, 8, 388650253, 388650319, 1176845762);
        while (write_ringbuffer(rb_all_flow, &t_kern, sizeof(tuple_t)) < 0) {}; //write to ringbuffer
    }

}

void LibPcapHelper::close() {
    //free the ringbuffer
    close_ringbuffer_shm(rb_all_flow);
    //free the pcap handler
    pcap_close(this->ph);
}
