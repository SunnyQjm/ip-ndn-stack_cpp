#!/bin/bash
name=$1
network=$2
site=$3
router=$4

HOME="."

killall nlsr
$HOME/nlsr_configs/generate.sh ${name} ${network} ${site} ${router} $HOME
mv nlsr.conf /usr/local/etc/ndn/nlsr.conf
nlsr -f /usr/local/etc/ndn/nlsr.conf 
