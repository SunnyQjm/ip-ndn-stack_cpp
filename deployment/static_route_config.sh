#!/bin/bash
ip=$1
faceId=$2

###################################
#### 本脚本用于协助配置最小原型的静态路由
###################################

sudo nfdc route add prefix /IP/pre/${ip} nexthop ${faceId}
sudo nfdc route add prefix /IP/${ip} nexthop ${faceId}
sudo nfdc route add prefix /IP/TCP/pre/${ip} nexthop ${faceId}
sudo nfdc route add prefix /IP/TCP/${ip} nexthop ${faceId}