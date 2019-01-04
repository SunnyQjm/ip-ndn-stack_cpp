cmake_minimum_required(VERSION 3.5)
project(ip_ndn_stack_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(libpcapcapture)
include_directories(helper)

add_executable(ip_ndn_stack_cpp main.cpp helper/JSONCPPHelper.cpp helper/LibPcapHelper.cpp helper/MapCacheHelper.cpp
        helper/NDNHelper.cpp helper/RawSocketHelper.cpp helper/SetHelper.cpp libpcapcapture/packet.cpp helper/PcapHelper.cpp)

target_link_libraries(ip_ndn_stack_cpp pcap ndn-cxx boost_system boost_thread jsoncpp pthread)
