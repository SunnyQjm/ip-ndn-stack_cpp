#!/bin/bash

# 当前节点的名字
nodeName=$1

##########################################
### 确保一个文件夹存在，不存在则创建
### @param dir
##########################################
function ensureDir() {
    dir=$1
    if [[ ! -d "$dir" ]]; then
        echo "文件夹${dir}不存在，正在创建"
        mkdir -p ${dir}
    fi
}

TEMP_DIR='./temp'

# 创建temp文件夹存放中转数据
ensureDir ${TEMP_DIR}



# 读取网络拓扑配置
json=`cat lab_topology.json`

# 保存配置列表
list=`echo ${json} | jq '.'`

# 获取网络拓扑的节点个数
length=`echo ${json} | jq '.|length'`
length=$(( $length - 1 ))

# 将节点信息和邻居信息输出到temp文件夹
for index in `seq 0 ${length}`
do
    rName=$(echo ${list} | jq -r ".[$index].name")
    nbs=$(echo ${list} | jq  ".[$index].nbs")
    nics=$(echo ${list} | jq ".[$index].nics")

    transName=$(echo ${rName//\//.})
    transName=$(echo ${transName#.})
    echo ${nics} | jq '.' > ${TEMP_DIR}/${transName}.nics
    echo ${nbs} | jq '.' > ${TEMP_DIR}/${transName}.nbs
done

transName=$(echo ${nodeName//\//.})
transName=$(echo ${transName#.})

# 当前节点的邻居信息
myNbs=$(cat ${TEMP_DIR}/${transName}.nbs)
myNics=$(cat ${TEMP_DIR}/${transName}.nics)

echo myNbs:
echo ${myNbs} | jq '.'
echo myNics:
echo ${myNics} | jq '.'
myNbsNum=`echo ${myNbs} | jq '.|length'`
let 'myNbsNum=myNbsNum-1'


echo ~~begin deal neighbour: ${myNbsNum}~~
for i in `seq 0 ${myNbsNum}`
do
    echo ${i};
    # 得到邻居的名字
    neighbourName=$(echo ${myNbs} | jq -r ".[$i].name")
    myIndex=$(echo ${myNbs} | jq -r ".[$i].nicIndex")

    localUri=$(echo ${myNbs} | jq -r ".[$i].local")
    remoteUri=$(echo ${myNbs} | jq -r ".[$i].target")

    # 如果存在Face则删除
    nfdc face destroy ${remoteUri}

    # 为该邻居创建Face
    echo "正在为邻居${neighbourName}创建Face"
    if [[ -z "${localUri}" ]]; then
#        echo "local uri empty: ${remoteUri}"
        echo "nfdc face create remote ${remoteUri}"
        nfdc face create remote ${remoteUri}
    fi

    if [[ -n "${localUri}" ]]; then
#        echo "local uri not empty: ${remoteUri}"
        echo "nfdc face create remote ${remoteUri} local ${localUri}"
        nfdc face create remote ${remoteUri} local ${localUri}
    fi
done
