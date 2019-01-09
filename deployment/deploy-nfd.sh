#!/bin/bash

####################################################
######## 本脚本用于安装运行NFD需要的环境
###################################################$


NDN_CXX_VERSION=0.6.3
NDN_NFD_VERSION=0.6.4
WEB_SOCKET_PP_VERSION=0.7.0
CHRONO_SYNC_VERSION=0.5.2
NLSR_VERSION=0.4.4_que
NDN_TOOLS_VERSION=0.6.2
DEFAULT_DIR=~/Documents

function ensureDir() {
    dir=$1
    if [[ ! -d "$dir" ]]; then
        echo "文件夹${dir}不存在，正在创建"
        mkdir -p ${dir}
    fi
}

# 确保Documents文件夹存在
ensureDir ${DEFAULT_DIR}

function cloneOrUpdate() {
    cd ${DEFAULT_DIR}
    name=$1
    url=$2
    if [[ -d ${name} ]];then
        cd ${name}
    else
        mkdir ${name}
        curl -L ${url} > ${name}.tar.gz
        tar xf ${name}.tar.gz -C ${name} --strip 1
        cd ${name}
    fi
}

# install nfd use apt
#sudo apt-get install software-properties-common -y
#sudo add-apt-repository ppa:named-data/ppa
#sudo apt update
#sudo apt-get install nfd -y

# install ndn-cxx prerequesites
sudo apt-get install build-essential libsqlite3-dev libboost-all-dev libssl-dev curl tar -y
sudo apt-get install doxygen graphviz python-sphinx python-pip -y
sudo pip install sphinxcontrib-doxylink sphinxcontrib-googleanalytics

# install ndn-cxx
cloneOrUpdate ndn-cxx https://github.com/named-data/ndn-cxx/archive/ndn-cxx-${NDN_CXX_VERSION}.tar.gz
./waf configure --enable-static
./waf
sudo ./waf install

# install nfd
sudo apt-get install build-essential pkg-config libboost-all-dev \
                     libsqlite3-dev libssl-dev libpcap-dev -y
sudo apt-get install doxygen graphviz python-sphinx -y
cloneOrUpdate NFD https://github.com/named-data/NFD/archive/NFD-${NDN_NFD_VERSION}.tar.gz
mkdir websocketpp
curl -L https://github.com/zaphoyd/websocketpp/archive/${WEB_SOCKET_PP_VERSION}.tar.gz > websocket.tar.gz
tar xf websocket.tar.gz -C websocketpp/ --strip 1
./waf configure
./waf
sudo ./waf install
#check nfd.conf exists?
if [[ ! -f /usr/local/etc/ndn/nfd.conf ]];then
    cd /usr/local/etc/ndn
    sudo cp nfd.conf.sample nfd.conf
fi

# install ChronoSync
cloneOrUpdate ChronoSync https://github.com/named-data/ChronoSync/archive/${CHRONO_SYNC_VERSION}.tar.gz
./waf configure
./waf
sudo ./waf install

# install NLSR
cloneOrUpdate NLSR https://github.com/SunnyQjm/NLSR/archive/NLSR-${NLSR_VERSION}.tar.gz
./waf configure
./waf
sudo ./waf install
#check nlsr.conf exists?
if [[ ! -f /usr/local/etc/ndn/nlsr.conf ]];then
    cd /usr/local/etc/ndn
    sudo cp nlsr.conf.sample nlsr.conf
fi
#check /var/lib/nlsr exists?
if [[ ! -d /var/lib/nlsr ]];then
    sudo mkdir /var/lib/nlsr
fi

# install ndn-tools
sudo apt-get install libpcap-dev -y
cloneOrUpdate ndn-tools https://github.com/named-data/ndn-tools/archive/ndn-tools-${NDN_TOOLS_VERSION}.tar.gz
./waf configure
./waf
sudo ./waf install

## install ndn-cpp
#sudo apt install build-essential libssl-dev libsqlite3-dev libprotobuf-dev protobuf-compiler \
#    liblog4cxx-dev doxygen libboost-all-dev -y
#cloneOrUpdate ndn-cpp https://github.com/named-data/ndn-cpp/archive/v0.15.tar.gz
#./configure
#make
#sudo make install

# install jsoncpp
sudo apt install libjsoncpp-dev -y
# enforce loading lib
sudo ldconfig