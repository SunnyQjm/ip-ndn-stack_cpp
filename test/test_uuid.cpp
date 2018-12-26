//
// Created by mingj on 18-12-26.
//

#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <ctime>

using namespace std;
/**
 * 随机生成一个uuid
 * @return
 */
string generateUUID() {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); // 这里是两个() ，因为这里是调用的 () 的运算符重载
    const string tmp_uuid = boost::uuids::to_string(a_uuid);
    return tmp_uuid;
}


/**
 * @return
 */
int main() {
    long start = time(nullptr);
    for(int i = 0; i < 10000; i++) {
        cout << generateUUID() << endl;
    }
    long end = time(nullptr);
    cout << (end - start) << endl;
}