#!/bin/bash
name=$1
network=$2
site=$3
router=$4
path=$5


cat /dev/null > nlsr.conf
${path}/nlsr_configs/generate-general.sh ${network} ${site} ${router} >> nlsr.conf
${path}/nlsr_configs/generate-neighbour.sh ${name} ${path} >> nlsr.conf
${path}/nlsr_configs/generate-hyperbolic.sh >> nlsr.conf
${path}/nlsr_configs/generate-fib.sh >> nlsr.conf
${path}/nlsr_configs/generate-security.sh >> nlsr.conf
