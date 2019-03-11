#!/bin/bash

username=$1
password=$2
ip=$3
routerName=$4
mapPort=$5
index=$6
branch_name=$7

PROJ_DIR=/home/${username}/Documents/ip-ndn-stack_cpp
DEPLOY_DIR=${PROJ_DIR}/deployment

#./doDeploy.sh $username $password $ip $routerName $mapPort $index
#exit
/usr/bin/expect << EOD
set timeout -1
spawn ssh root@${ip} -p${mapPort} "
cd ${PROJ_DIR}
git pull
git checkout -b ${branch_name} origin/${branch_name}
git checkout ${branch_name}
"
expect {
    "(yes/no)?" {
        send "yes\r";
        expect "password:";
        send "${password}\r";
        exp_continue;
    }
    "password:" {
        send "${password}\r";
        exp_continue;
    }
}
EOD