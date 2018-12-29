//
// Created by mingj on 18-12-28.
//

#ifndef IP_NDN_STACK_CPP_SETHELPER_H
#define IP_NDN_STACK_CPP_SETHELPER_H
#include <iostream>
#include <set>
#include <boost/thread/pthread/shared_mutex.hpp>

template <class T>
class SetHelper {
public:
    bool saveConcurrence(const T &t);
    bool save(const T &t);
    bool find(const T &t);
    bool erase(const T &t);
private:
    set<T> s;
    boost::shared_mutex insertMutex;
    boost::shared_mutex deleteMutex;
    boost::shared_mutex findMutex;
};

/**
 * (并发)插入一个数据到set当中（本操作是线程安全的）
 * 
 * 当且仅当t不存在于set中，且插入成功的情况下才返回true，否则返回false
 * @tparam T 
 * @param t 
 * @return 
 */
template<class T>
bool SetHelper<T>::saveConcurrence(const T &t) {
    insertMutex.lock();
    auto res = this->s.insert(t);
    insertMutex.unlock();
    return res.second;
}

/**
 * 插入一个数据到set当中（本操作是非线程安全的）
 *
 * 当且仅当t不存在于set中，且插入成功的情况下才返回true，否则返回false
 * @tparam T
 * @param t
 * @return
 */
template<class T>
bool SetHelper<T>::save(const T &t) {
    return this->s.insert(t).second;
}

/**
 * 判断一个值是否存在于set当中
 * @tparam T
 * @param t
 * @return
 */
template<class T>
bool SetHelper<T>::find(const T &t) {
    insertMutex.lock_shared();
    auto res = (s.find(t) != s.end());
    insertMutex.unlock_shared();
    return res;
}

/**
 * 删除一个指定元素
 * @tparam T
 * @param t
 * @return
 */
template<class T>
bool SetHelper<T>::erase(const T &t) {
    insertMutex.lock();
    s.erase(t);
    insertMutex.unlock();
    return false;
}


#endif //IP_NDN_STACK_CPP_SETHELPER_H
