#!/bin/bash
name=$1
path=$2

routerName=$(echo ${name#/ndn/edu/pkusz})
cat /dev/null > nlsr.conf
${path}/nlsr_configs/generate-general.sh /ndn /edu/pkusz ${routerName} >> nlsr.conf
${path}/nlsr_configs/generate-neighbour.sh ${name} ${path} >> nlsr.conf
${path}/nlsr_configs/generate-hyperbolic.sh >> nlsr.conf
${path}/nlsr_configs/generate-fib.sh >> nlsr.conf
${path}/nlsr_configs/generate-security.sh >> nlsr.conf
