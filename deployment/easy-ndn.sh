#!/bin/bash
nodeName=$1
ip=$2

sudo ./createFace.sh ${nodeName}
sudo ./start-nlsr.sh ${nodeName} &
sudo ./nlsr_advertise_helper.sh ${ip}