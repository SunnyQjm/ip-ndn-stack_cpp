//
// Created by mingj on 18-12-23.
//

#include "LibPcapHelper.h"


double time2dbl(struct timeval time_value) {
    double new_time = 0;
    new_time = (double) (time_value.tv_usec);
    new_time /= 1000000;
    new_time += (double) time_value.tv_sec;
    return (new_time);

}


LibPcapHelper::LibPcapHelper() {

}

void LibPcapHelper::bindNDNHelper(NDNHelper *ndnHelper) {
    this->ndnHelper = ndnHelper;
}

void LibPcapHelper::bindCacheHelper(MapCacheHelper<tuple_p > *cacheHelper) {
    this->cacheHelper = cacheHelper;
}

void LibPcapHelper::bindPendingInterestTable(MapCacheHelper<time_t> *pendingInterestMap) {
    this->pendingInterestTable = pendingInterestMap;
}


void LibPcapHelper::bindSequenceTable(MapCacheHelper<int> *sequenceTable) {
    this->sequenceTable = sequenceTable;
}

void LibPcapHelper::initLibPcap(string configFilePath) {
    cout << "init libpcap" << endl;
    JSONCPPHelper jsoncppHelper(configFilePath);
    //    interval_len = conf_common_interval_len(conf);  //the value of interva    l_len is the "upload period" in config.ini, in ms
    char tmp[1024];
    string dev_name = jsoncppHelper.getString("pcap_if");

    //const char *dev_name = pcap_lookupdev(NULL);//the value of dev_name is     the interface used for packet capturing in config.ini
    struct pcap_pkthdr *header = nullptr;
    const uint8_t *pkt = nullptr;
    char ebuf[PCAP_ERRBUF_SIZE];

    int bufsize = jsoncppHelper.getInt(
            "pcap_buf_size"); //the value of bufsize is     the the capturing buffer size in config.ini
    int snaplen = jsoncppHelper.getInt(
            "pcap_snap_len"); //the value of snaplen is     the snapshot length for pcap handler in config.ini
    int to_ms = jsoncppHelper.getInt("pcap_to_ms"); //the value of to_ms is the read timeout for pcap handler, in ms
    //init ringbuffer (there are two ringbuffers)
    sprintf(tmp, "pub1%02d", 0); //fomat the tmp characters.
    tuple_p t_kern;
    t_kern = new tuple_t();     //动态分配内存
    memset(t_kern, 0, sizeof(struct Tuple));

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

    struct bpf_program filter{};
    pcap_compile(ph, &filter, filter_app.c_str(), 0, net);
    pcap_setfilter(ph, &filter);
    //capture packets and copy the packets to the ringbuffer
    cout << "main thread id: " << boost::this_thread::get_id() << endl;
    while ((res = pcap_next_ex(ph, &header, &pkt)) >=
           0) { //reads the next packet and returns a success/failure indication.
        if (pkt == nullptr || res == 0) {
            continue;
        }
        //char filter_app[] = "ether dst 00:1e:67:83:0c:0a";

//        boost::thread t(bind(&LibPcapHelper::deal, this, header, pkt));

        this->deal(header, pkt);
    }

}

void LibPcapHelper::close() {
    //free the pcap handler
    pcap_close(this->ph);
}

void LibPcapHelper::join() {
//    pthread_join(this->readRingBufferThreadId, nullptr);
}

void LibPcapHelper::deal(const void *arg1, const void *arg2) {
//    cout << "启动线程" << boost::this_thread::get_id() << endl;
    struct pcap_pkthdr *header = (struct pcap_pkthdr *) arg1;
    const uint8_t *pkt = (const uint8_t *) arg2;
    tuple_p tuple = new tuple_t();
    double pkt_ts = time2dbl(header->ts); //doubleֵ

    //decode the captured packet
    decode(pkt, header->caplen, header->len, pkt_ts, tuple);
	if (tuple->key.proto == IPPROTO_TCP) {
		string key;
		// 先查找自增序列表
		uint32_t int_sip = ntohl(tuple->key.src_ip);
		uint32_t int_dip = ntohl(tuple->key.dst_ip);
		//cout<<int_sip<<endl;
		string sip1 = to_string((int_sip >> 24) & 0xFF);
		string sip2 = to_string((int_sip >> 16) & 0xFF);
		string sip3 = to_string((int_sip >> 8) & 0xFF);
		string sip4 = to_string((int_sip) & 0xFF);
		string dip1 = to_string((int_dip >> 24) & 0xFF);
		string dip2 = to_string((int_dip >> 16) & 0xFF);
		string dip3 = to_string((int_dip >> 8) & 0xFF);
		string dip4 = to_string((int_dip) & 0xFF);

		key.append(dip1);
		key.append(".");
		key.append(dip2);
		key.append(".");
		key.append(dip3);
		key.append(".");
		key.append(dip4);
		key.append("/");

		key.append(sip1);
		key.append(".");
		key.append(sip2);
		key.append(".");
		key.append(sip3);
		key.append(".");
		key.append(sip4);
		key.append("/");
		uint16_t int_sport = ntohl(tuple->key.src_port);
		uint16_t int_dport = ntohl(tuple->key.dst_port);
		string dport = to_string((int_dport >> 16) & 0xFFFF);
		string sport = to_string((int_sport >> 16) & 0xFFFF);
	
		
		key.append(dport);
		key.append("/");
		key.append(sport);
		key.append("/");
		
		auto res = sequenceTable->get(key);

		if (!res.second) {//若不存在则将index即自增表的value设为1并插入；再存入缓存中
			tuple->index = 1;
			auto result_seq = sequenceTable->save(key, tuple->index);
			if (!result_seq) {
				cout << "插入失败" << endl;
				return;
			}
			//string id = to_string((tuple->index >> 32) & 0xFFFF);
			string uuid;
			uuid.append(dport);
			uuid.append("_");
			uuid.append(sport);
			uuid.append("_");
			uuid.append("1");
			auto result_cache = cacheHelper->save(uuid, tuple);
			if (!result_cache) {
				cout << "插入失败" << endl;
				return;
			}

			//发送预请求兴趣包

			string name = "IP/TCP/pre/";
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

			ndnHelper->expressInterest(name);
			return;
		}
		else {//若存在则将index的值++，并查找悬而未决表
			tuple->index++;
			//res.second = tuple->index;
			string id1 = to_string((tuple->index >> 32) & 0xFFFF);
			string id2 = to_string((tuple->index >> 16) & 0xFFFF);
			string uuid;
			uuid.append(dport);
			uuid.append("_");
			uuid.append(sport);
			uuid.append("_");
			uuid.append("id1");
			uuid.append("id2");
			string formal_name = "IP/TCP/";
			//char * ip = (char*) (&t.key.src_ip);
			formal_name.append(dip1);
			formal_name.append(".");
			formal_name.append(dip2);
			formal_name.append(".");
			formal_name.append(dip3);
			formal_name.append(".");
			formal_name.append(dip4);
			formal_name.append("/");

			formal_name.append(sip1);
			formal_name.append(".");
			formal_name.append(sip2);
			formal_name.append(".");
			formal_name.append(sip3);
			formal_name.append(".");
			formal_name.append(sip4);
			formal_name.append("/");

			formal_name.append(uuid);
			auto formal_res = pendingInterestMap->get(formal_name);
			timespec time1;
			clock_gettime(CLOCK_MONOTONIC, &time1);
			if (formal_res.second >= time1.tv_sec * 1000000000LL + time1.tv_nsec) {//如果找到相应表项，若时间在有效期内，则直接发送date包
				ndnHelper->putData(formal_name, tuple->pkt);//修改
			}
			else {//未找到或则时间失效则将数据进行缓存并发送预请求兴趣包并删除相应表项
				string id1 = to_string((tuple->index >> 32) & 0xFFFF);
				string id2 = to_string((tuple->index >> 16) & 0xFFFF);
				string uuid;
				uuid.append(dport);
				uuid.append("_");
				uuid.append(sport);
				uuid.append("_");
				uuid.append("id1");
				uuid.append("id2");
				auto result_cache = cacheHelper->save(uuid, tuple);
				if (!result_cache) {
					cout << "插入失败" << endl;
					return;
				}

				//发送预请求兴趣包

				string name = "IP/TCP/pre/";
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

				ndnHelper->expressInterest(name);
				pendingInterestMap->erase(formal_name);          //删除相应悬而未决表表项
				return;
			}
		}
	//	tuple_p tuple1 = res.first;
	}
	else {//为其他协议包用原来的方式传输
		string uuid = this->generateUUID();
		auto result = cacheHelper->save(uuid, tuple);
		if (!result) {
			cout << "插入失败" << endl;
			return;
		}

		//发送兴趣包
		uint32_t int_sip = ntohl(tuple->key.src_ip);
		uint32_t int_dip = ntohl(tuple->key.dst_ip);
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

		ndnHelper->expressInterest(name);
		//    cout << "线程结束" << boost::this_thread::get_id() << endl;

	}
   
}

/**
 * 随机生成一个uuid
 * @return
 */
string LibPcapHelper::generateUUID() {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); // 这里是两个() ，因为这里是调用的 () 的运算符重载
    const string tmp_uuid = boost::uuids::to_string(a_uuid);
    return tmp_uuid;
}



