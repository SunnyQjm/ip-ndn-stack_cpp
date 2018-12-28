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

private:
    set<T> s;
    boost::shared_mutex insertMutex;
    boost::shared_mutex deleteMutex;
};

/**
 * 插入一个数据到set当中
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


#endif //IP_NDN_STACK_CPP_SETHELPER_H
