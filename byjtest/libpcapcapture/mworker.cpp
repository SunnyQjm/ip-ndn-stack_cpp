#include "ndn.h"


//byj-----------------------------------begin
void *init(void* arg) {
	KeyChain KeyChain_;
	face.setCommandSigningInfo(KeyChain_, KeyChain_.getDefaultCertificateName());
    Name register_prefix1("/IP/192.169.1.1");
    Name register_prefix2("/IP/pre/192.169.1.1");
	face.registerPrefix(register_prefix1,(const OnInterestCallback&)bind(&Producer::onInterest,&producer, _1, _2, _3, _4, _5),bind(&Producer::onRegisterFailed,&producer, _1));
	face.registerPrefix(register_prefix2,(const OnInterestCallback&)bind(&Producer::onInterest,&producer, _1, _2, _3, _4, _5),bind(&Producer::onRegisterFailed,&producer, _1));
	while(1) {
		face.processEvents();
		usleep(50);
	}
}
//byj-----------------------------------end


extern conf_t* conf;
//init variables for ringbuff
ringbuffer_t* rb_all_flow;
pthread_t all_flow_thread;
//transform timeval to double
double time2dbl(struct timeval time_value) {
	     double new_time = 0;
	     new_time = (double)(time_value.tv_usec);
	     new_time /= 1000000;
	     new_time += (double)time_value.tv_sec;
	     return(new_time);
	
}
 //all-flow function
static void* get_ip_fun(void *arg) {
	     tuple_t t;
	     memset(&t, 0, sizeof(struct Tuple));
		 while (1) {
			 while (read_ringbuffer(rb_all_flow, &t) < 0) {}; //read the ringbuff  
			 flow_key_t *k;
			 k = (flow_key_t*)malloc(sizeof(flow_key_t));
			 memcpy(k, &t.key, sizeof(flow_key_t));
			// printf("144 tuple sip:%d ,dip:%d ,flag:%d ,size:%d\n ,uid:%d\n", t.key.src_ip, t.key.dst_ip, t.flag, t.size, t.index);
             uint32_t int_sip = ntohl(t.key.src_ip);
             uint32_t int_dip = ntohl(t.key.dst_ip);
             //cout<<int_sip<<endl;
             string sip1 = to_string((int_sip >> 24) & 0xFF);
			 string sip2 = to_string((int_sip >> 16) & 0xFF);
			 string sip3 = to_string((int_sip >> 8) & 0xFF);
			 string sip4 = to_string((int_sip) & 0xFF);
			 string dip1 = to_string((int_dip >> 24) & 0xFF);
			 string dip2 = to_string((int_dip >> 16) & 0xFF);
			 string dip3 = to_string((int_dip >> 8) & 0xFF);
			 string dip4 = to_string((int_dip) & 0xFF);
			 string uuid = to_string(t.index);
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
			 free(k);
		 }
		 pthread_exit(NULL);	
 }
 int main(int argc, char *argv[]) {
//	Face face("localhost");
//	Name register_prefix1("/IP/192.169.1.1");
//	Name register_prefix2("/IP/pre/192.169.1.1");

	pthread_t threadId;   //byj
	pthread_create(&threadId, NULL, init, NULL);    //byj
	 //printf("%d",sizeof(fe_t));
		 //return 0;
	 if (argc < 2) {
		 fprintf(stderr, "Usage: %s [config file]\n", argv[0]);
		 exit(-1);
	 }
	 //init config and redis_ctx
	 conf = Config_Init(argv[1]); //get the content of config.ini
     //    interval_len = conf_common_interval_len(conf);  //the value of interva    l_len is the "upload period" in config.ini, in ms
	 char tmp[1024];
	 const char *dev_name = conf_common_pcap_if(conf);
	 //const char *dev_name = pcap_lookupdev(NULL);//the value of dev_name is     the interface used for packet capturing in config.ini
	 struct pcap_pkthdr *header;
	 const u_char *pkt;
	 char ebuf[PCAP_ERRBUF_SIZE];
	 int bufsize = conf_common_pcap_bufsize(conf); //the value of bufsize is     the the capturing buffer size in config.ini
	 int snaplen = conf_common_pcap_snaplen(conf); //the value of snaplen is     the snapshot length for pcap handler in config.ini
	 int to_ms = conf_common_pcap_toms(conf); //the value of to_ms is the rea    d timeout for pcap handler, in ms
											  //init ringbuffer (there are two ringbuffers)
	 sprintf(tmp, "pub1%02d", 0); //fomat the tmp characters.
	 rb_all_flow = create_ringbuffer_shm1(tmp, sizeof(tuple_t));  //create a ri    ngbuffer
	 tuple_t t_kern;
	 double pkt_ts;
	 memset(&t_kern, 0, sizeof(struct Tuple));
	 int res;
	 pthread_attr_t attr1;
	 int s1 = pthread_attr_init(&attr1); //initialize
	 if (s1 != 0) {
		 LOG_ERR("pthread_attr_init: %s\n", strerror(errno));
	 }
	 s1 = pthread_create(&all_flow_thread, &attr1, &get_ip_fun, NULL); //crea    te a thread
	 if (s1 != 0) {
		 LOG_ERR("pthread_creat: %s\n", strerror(errno));
	 }
	 //init pcap
	 pcap_t *ph = pcap_create(dev_name, ebuf); //dev_name is the network devic    e to open, ebuf is error buffer
	 if (ph == NULL) {
		 pcap_close(ph);
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
	 pcap_lookupnet(dev_name, &net, &mask, ebuf);
	 //capture packets and copy the packets to the ringbuffer
	 while ((res = pcap_next_ex(ph, &header, &pkt)) >= 0) { //reads the next packe    t and returns a success/failure indication.
		 if (pkt != NULL) {
			 if (res == 0)
				 continue;
		 }
		 //decode the captured packet
		 const char *filter_app = conf_common_pcap_dstmac(conf);
		 //char filter_app[] = "ether dst 00:1e:67:83:0c:0a";
		 struct bpf_program filter;
		 pcap_compile(ph, &filter, filter_app, 0, net);
		 pcap_setfilter(ph, &filter);
		 pkt_ts = time2dbl(header->ts); //doubleֵ
		// 
		 decode(pkt, header->caplen, header->len, pkt_ts, &t_kern);
                 t_kern.index = AwareHash((uint8_t*)t_kern.pkt, 8, 388650253, 388650319, 1176845762);
		 while (write_ringbuffer(rb_all_flow, &t_kern, sizeof(tuple_t)) < 0) {}; //write to ringbuffer
	 }
	 pthread_join(all_flow_thread, NULL);  //join these to threads
	 pthread_join(threadId,NULL);    //byj
	 //free the ringbuffer
	 close_ringbuffer_shm(rb_all_flow);
	 //free the pcap handler
	 pcap_close(ph);
	 Config_Destroy(conf);
	 return 0;
 }
                          

