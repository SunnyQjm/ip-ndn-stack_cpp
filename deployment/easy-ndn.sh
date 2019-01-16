#!/bin/bash
nodeName=$1
ip=$2
configPath=$3

sudo ./createFace.sh ${nodeName}
sudo ./start-nlsr.sh ${nodeName} &
sleep 1
sudo ./nlsr_advertise_helper.sh ${ip}
sudo ../ip_ndn_stack_cpp ${configPath}