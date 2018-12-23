//
// Created by mingj on 18-12-23.
//
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>

using namespace std;
int main() {
    string s = "/IP/pre/192.169.1.4/192.169.1.1/15645641645";
    vector<string> v;
    boost::split(v, s, boost::is_any_of("/"));
    for(int i = 0; i < v.size(); i++) {
        cout << v[i] << " ; "<< endl;
    }
}