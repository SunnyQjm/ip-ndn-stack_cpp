//
// Created by mingj on 18-12-18.
//
#include "./helper/NDNHelper.h"
#include "./helper/JSONCPPHelper.h"
#include "./helper/LibPcapHelper.h"

#include <boost/thread.hpp>
using namespace std;

void dealNDN(void *arg) {
    auto *ndnHelper = (NDNHelper *) arg;
    cout << "threadId： " << boost::this_thread::get_id() << endl;
    ndnHelper->start();
}
int main(int argc, char *argv[]) {

    NDNHelper ndnHelper;
    CacheHelper cacheHelper;
    LibPcapHelper libPcapHelper;

    ndnHelper.bindCacheHelper(&cacheHelper);
    libPcapHelper.bindCacheHelper(&cacheHelper);
    libPcapHelper.bindNDNHelper(&ndnHelper);


    ndnHelper.initNDN(argv[1]);
    ndnHelper.start();
    cout << "threadId： " << boost::this_thread::get_id() << endl;
    boost::thread t(bind(&dealNDN, &ndnHelper));

    libPcapHelper.initLibPcap(argv[1]);

    t.join();
    libPcapHelper.join();
    ndnHelper.join();
    libPcapHelper.close();


    return 0;
}

