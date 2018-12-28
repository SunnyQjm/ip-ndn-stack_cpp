//
// Created by mingj on 18-12-25.
//

#ifndef IP_NDN_STACK_CPP_CACHEHELPER_H
#define IP_NDN_STACK_CPP_CACHEHELPER_H

#include "../libpcapcapture/libpcap.h"
#include <unordered_map>
#include <iostream>
#include <boost/thread/pthread/shared_mutex.hpp>

using namespace std;

template <class T>
class MapCacheHelper {
public:
    MapCacheHelper(){}
    bool save(string key, T value);
    pair<T, bool> get(string key);
    unsigned long erase(string key);
    unsigned long erase(string key, T tuple);
private:
    unordered_map<string, tuple_p> ipPacketCache;
    T emptyItem;
    boost::shared_mutex insertMutex;
    boost::shared_mutex deleteMutex;
};

/**
 * 插入一个数据
 * @tparam T
 * @param key
 * @param value
 * @return
 */
template<class T>
bool MapCacheHelper<T>::save(string key, T value) {
    insertMutex.lock();
    auto res = this->ipPacketCache.insert(make_pair(key, value));
    insertMutex.unlock();
    return res.second;
}

/**
 * 获取一个数据
 * @tparam T
 * @param key
 * @return
 */
template<class T>
pair<T, bool> MapCacheHelper<T>::get(string key) {
    auto count = this->ipPacketCache.count(key);
    if(count == 0) {    //缓存中不存在该键值
        return make_pair(this->emptyItem, false);
    }
    auto result = this->ipPacketCache.find(key);
    return make_pair(result->second, true);
}

/**
 * 删除一个key对应的数据
 * @tparam T
 * @param key
 * @return
 */
template<class T>
unsigned long MapCacheHelper<T>::erase(string key) {
    deleteMutex.lock();
    auto res = this->ipPacketCache.erase(key);
    deleteMutex.unlock();
    return res;
}

/**
 * 删除一个数据，病释放其内存，只有当T的类型是指针类型的时候才可执行此操作
 * @tparam T
 * @param key
 * @param tuple
 * @return
 */
template<class T>
unsigned long MapCacheHelper<T>::erase(string key, T tuple) {
    deleteMutex.lock();
    delete tuple;
    auto res = this->ipPacketCache.erase(key);
    deleteMutex.unlock();
    return res;
}


#endif //IP_NDN_STACK_CPP_CACHEHELPER_H
