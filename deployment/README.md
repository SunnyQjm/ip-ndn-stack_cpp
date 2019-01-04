> # IP_NDN_STACK 部署脚本

- ## 安装环境
    ```bash
    sudo ./deploy-nfd.sh
    ```
    执行上面的代码安装nfd所需要的环境，以及 ip-ndn-stack 项目所需要的依赖库

- ## 准备网络拓扑配置信息
    网络的基本拓扑信息放在 `lab_topology_raw.json` 当中，其中每一个节点的信息格式如下（包含本节点的网卡信息和邻居的信息）：
    ```json
    {
            "name":"/ndn/edu/pkusz/pkun1",
            "network":"/ndn",
            "site":"/edu/pkusz",
            "router":"/pkun1",
            "id":0,
            "ip":"lab.pkusz.qjm253.cn",
            "mapPort":9751,
            "username":"pkun1",
            "password":"26035354",
            "nics":[
                {
                    "name":"enp2s0f0",
                    "mac":"a0:36:9f:85:bc:8c"
                },
                {
                    "name":"enp2s0f1",
                    "mac":"a0:36:9f:85:bc:8d"
                },
                {
                    "name":"enp2s0f2",
                    "mac":"a0:36:9f:85:bc:8e"
                },
                {
                    "name":"enp2s0f3",
                    "mac":"a0:36:9f:85:bc:8f"
                },
                {
                    "name":"enp3s0",
                    "mac":"00:24:1d:2c:f0:09"
                }
            ],
            "nbs":[
                {
                    "name":"/ndn/edu/pkusz/pkun3",
                    "network":"/ndn",
                    "site":"/edu/pkusz",
                    "router":"/pkun3",
                    "nicIndex":0
                },
                {
                    "name":"/ndn/edu/pkusz/pkun2",
                    "network":"/ndn",
                    "site":"/edu/pkusz",
                    "router":"/pkun2",
                    "nicIndex":1
                },
                {
                    "name":"/ndn/edu/pkusz/main2",
                    "network":"/ndn",
                    "site":"/edu/pkusz",
                    "router":"/mainn2",
                    "nicIndex":2
                }
            ]
        }
    ```
    
    在使用之前需要调用nodejs脚本来初始化，基于 `lab_topology_raw.json` 生成 `lab_topology.json`
    > 如果 `lab_topology.json` 存在且不需要更新，可以不用执行下面的脚本
    
    ```bash
    node dealLabTopology.js lab_topology_raw.json | jq '.' > lab_topology.json
    ```
    
    如果要执行上述操作，但是没有安装Nodejs，需要先安装Nodejs，安装方法详见[Nodejs包管理器安装方法](https://nodejs.org/en/download/package-manager/)
    
- ## NFD启动以及face创建
    - 启动NFD
        ```bash
        nfd-start
        ```
    - 为当前节点创建Face（是根据`lab_topology.json`中配置的各个节点间的关系配置的）
        ```bash
        sudo ./createFace.sh <nodeName>
        ```
        其中，`nodeName`为当前节点的标示名（即`lab_topology.json`配置文件中节点的`name`字段）


- ## 静态路由配置
    > 静态路由的配置，可以选择手动，也可以使用协助脚本（协助脚本只是简化工作）
    
   ```bash
   ./static_route_config.sh <ip> <faceId>
   ```
   
   假设ip等于 `192.169.1.4`，faceId为 `256`，则执行 `./static_route_config.sh 192.169.1.4 256` 等效于执行下面几行命令
   
   ```bash
   sudo nfdc route add prefix /IP/pre/192.169.1.4 nexthop 256
   sudo nfdc route add prefix /IP/192.169.1.4 nexthop 256
   sudo nfdc route add prefix /IP/TCP/pre/192.169.1.4 nexthop 256
   sudo nfdc route add prefix /IP/TCP/192.169.1.4 nexthop 256
   ```