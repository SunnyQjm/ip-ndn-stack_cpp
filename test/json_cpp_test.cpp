//
// Created by mingj on 18-12-22.
//

#include "../helper/JSONCPPHelper.cpp"

using namespace std;
int main(int argc, char *argv[]) {
    JSONCPPHelper jsoncppHelper(argv[1]);

    cout << jsoncppHelper.getString("name") << endl;
}
