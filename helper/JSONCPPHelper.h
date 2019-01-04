//
// Created by mingj on 18-12-22.
//
#ifndef IP_NDN_STACK_CPP_JSONCPPHELPER_H
#define IP_NDN_STACK_CPP_JSONCPPHELPER_H

#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>

using namespace std;
class JSONCPPHelper {
public:
    JSONCPPHelper(const string &filePath);
    string getString(string key);
    int getInt(string key);
    unsigned long long getull(string key);
    bool getBool(string key);
    double getDouble(string key);
    Json::Value get(string key);
private:
    Json::Value root;
};


#endif //IP_NDN_STACK_CPP_JSONCPPHELPER_H
