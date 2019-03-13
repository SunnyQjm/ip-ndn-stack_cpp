#!/bin/bash
nodeName=$1
network=$2
site=$3
router=$4
ip=$5
configPath=$6
paramNum=$#

sudo ./createFace.sh ${nodeName}
sudo ./start-nlsr.sh ${nodeName} ${network} ${site} ${router} &
sleep 1


if [[ ${paramNum} -ge 5 ]]; then
    sudo ./nlsr_advertise_helper.sh ${ip}
fi

if [[ ${paramNum} -eq 6 ]]; then
    sudo ../ip_ndn_stack_cpp ${configPath}
fi