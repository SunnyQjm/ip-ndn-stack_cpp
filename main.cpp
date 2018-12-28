//
// Created by mingj on 18-12-18.
//
#include "./helper/NDNHelper.h"
#include "./helper/JSONCPPHelper.h"
#include "./helper/LibPcapHelper.h"

using namespace std;

void dealNDN(void *arg) {
    auto *ndnHelper = (NDNHelper *) arg;
    ndnHelper->start();
}
int main(int argc, char *argv[]) {

    NDNHelper ndnHelper;
    MapCacheHelper cacheHelper;
    MapCacheHelper pendingInterestMap;
    LibPcapHelper libPcapHelper;

    ndnHelper.bindCacheHelper(&cacheHelper);
    ndnHelper.bindPendingInterestMap(&pendingInterestMap);
    libPcapHelper.bindCacheHelper(&cacheHelper);
    libPcapHelper.bindNDNHelper(&ndnHelper);


    ndnHelper.initNDN(argv[1]);

    boost::thread t(bind(&dealNDN, &ndnHelper));

    libPcapHelper.initLibPcap(argv[1]);

    t.join();
    libPcapHelper.join();
    ndnHelper.join();
    libPcapHelper.close();


    return 0;
}

