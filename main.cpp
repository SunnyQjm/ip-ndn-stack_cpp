//
// Created by mingj on 18-12-18.
//
#include "./helper/NDNHelper.h"
#include "./helper/JSONCPPHelper.h"
#include "./helper/LibPcapHelper.h"
using namespace std;

int main(int argc, char *argv[]) {

    NDNHelper ndnHelper;
    CacheHelper cacheHelper;
    LibPcapHelper libPcapHelper;

    ndnHelper.bindCacheHelper(cacheHelper);
    libPcapHelper.bindCacheHelper(cacheHelper);
    libPcapHelper.bindNDNHelper(ndnHelper);

    ndnHelper.initNDN(argv[1]);
    libPcapHelper.initLibPcap(argv[1]);

    libPcapHelper.join();
    ndnHelper.join();
    libPcapHelper.close();


    return 0;
}

