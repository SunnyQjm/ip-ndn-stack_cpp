//
// Created by mingj on 18-12-25.
//

#include "MapCacheHelper.h"
///**
// * 缓存一个包
// * @param key
// * @param value
// * @return
// */
//bool CacheHelper::save(string key, tuple_p value) {
//
//}
//
///**
// * 通过key查询一个包
// * @param key
// * @return
// */
//pair<tuple_p, bool> CacheHelper::get(string key) {
//    auto count = this->ipPacketCache.count(key);
//    if(count == 0) {    //缓存中不存在该键值
//        return make_pair(this->emptyTuple, false);
//    }
//    auto result = this->ipPacketCache.find(key);
//    return make_pair(result->second, true);
//}
//
///**
// * 删除
// * @param key
// * @return
// */
//unsigned int CacheHelper::erase(string key) {
//    deleteMutex.lock();
//    auto res = this->ipPacketCache.erase(key);
//    deleteMutex.unlock();
//    return res;
//}
//
//unsigned int CacheHelper::erase(string key, tuple_p tuple) {
//    deleteMutex.lock();
//    delete tuple;
//    auto res = this->ipPacketCache.erase(key);
//    deleteMutex.unlock();
//    return res;
//}
