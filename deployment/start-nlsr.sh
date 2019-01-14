#!/bin/bash
name=$1

HOME=.

killall nlsr
$HOME/nlsr_configs/generate.sh ${name} $HOME
mv nlsr.conf /usr/local/etc/ndn/nlsr.conf
nlsr -f /usr/local/etc/ndn/nlsr.conf 
