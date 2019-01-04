//
// Created by mingj on 18-12-18.
//
#include "NDNHelper.h"
#include "JSONCPPHelper.h"
#include "LibPcapHelper.h"

using namespace std;

void dealNDN(void *arg) {
    auto *ndnHelper = (NDNHelper *) arg;
    ndnHelper->start();
}
int main(int argc, char *argv[]) {

    NDNHelper ndnHelper;
    MapCacheHelper<tuple_p > cacheHelper;
    MapCacheHelper<long > pendingInterestMap;
    MapCacheHelper<int> sequenceTable;
    SetHelper<string> prefixRequestTable;
    LibPcapHelper libPcapHelper(argv[1]);

    ndnHelper.bindCacheHelper(&cacheHelper);
    ndnHelper.bindPendingInterestMap(&pendingInterestMap);
    ndnHelper.bindPrefixGuestTable(&prefixRequestTable);
    libPcapHelper.bindCacheHelper(&cacheHelper);
    libPcapHelper.bindPendingInterestTable(&pendingInterestMap);
    libPcapHelper.bindSequenceTable(&sequenceTable);
    libPcapHelper.bindNDNHelper(&ndnHelper);


    ndnHelper.initNDN(argv[1]);

    boost::thread t(bind(&dealNDN, &ndnHelper));

    libPcapHelper.start();

    t.join();
    ndnHelper.join();
    libPcapHelper.close();


    return 0;
}

