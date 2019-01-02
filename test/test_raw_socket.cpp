#include <iostream>
#include <ctime>
#include <chrono>
#include "../helper/RawSocketHelper.cpp"

using namespace std;
int main()
{
    uint8_t data[8000];
    RawSocketHelper rawSocketHelper;
    string ip = "123.207.96.66";
    auto start = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now()
            .time_since_epoch());
    for(int i = 0; i < 10000; i++) {
        rawSocketHelper.sendPacketTo(data, 8000, ip);
    }
    auto end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now()
            .time_since_epoch());
    cout << "start: " << start.count() << ", end: " << end.count() << endl << (end.count() - start.count()) << endl;
    return 0;
}

