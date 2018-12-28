modified by wangbohui 2018.3.20

the main function is mwoker.
before starting mwoker,you must start the redis server,or it will fail.

the main function of mwoker is :
1.capture the packet by pcap
2.process the raw packet
3.use hash table to handle the data
4.saveConcurrence data to redis
5.send the data to mserver by zmq
