//
// Created by mingj on 18-12-25.
//

#include "CacheHelper.h"
/**
 * 缓存一个包
 * @param key
 * @param value
 * @return
 */
bool CacheHelper::save(string key, tuple_t value) {
    return this->ipPacketCache.insert(make_pair(key, value)).second;
}

/**
 * 通过key查询一个包
 * @param key
 * @return
 */
pair<tuple_t, bool> CacheHelper::get(string key) {
    auto count = this->ipPacketCache.count(key);
    if(count == 0) {    //缓存中不存在该键值
        return make_pair(this->emptyTuple, false);
    }
    auto result = this->ipPacketCache.find(key);
    return make_pair(result->second, true);
}

unsigned int CacheHelper::erase(string key) {
    return this->ipPacketCache.erase(key);
}
