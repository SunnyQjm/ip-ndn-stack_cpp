//
// Created by mingj on 18-12-18.
//
#include "./helper/NDNHelper.h"
#include "./helper/JSONCPPHelper.h"
#include "./helper/LibPcapHelper.h"

#include <boost/thread.hpp>
using namespace std;

void dealNDN(const boost::shared_ptr<NDNHelper> ndnHelper) {
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
    boost::thread t(dealNDN, &ndnHelper);
    libPcapHelper.initLibPcap(argv[1]);

    libPcapHelper.join();
    ndnHelper.join();
    t.join();
    libPcapHelper.close();


    return 0;
}

