> ## 最小原型网络环境配置
> - 该配置临时生效，电脑重启之后会丢失
> - node1(192.169.1.1) -> node2(192.169.1.2) -> node3(192.169.1.3) -> pkusz1(192.169.1.4)

- ### 确保各NDN主机的nfd进程已经启动
  > 在本最小原型场景下，pkun1和pkun2为NDN主机，pkun3和hitn1为IP主机

  NDN主机的环境配置以及Face创建等，参见[IP_NDN_STACK 部署脚本](https://github.com/SunnyQjm/ip-ndn-stack_cpp/tree/master/deployment)

  ```bash
  nfd-status
  
  # 如果没有启动则执行下面的命令启动
  nfd-start
  ```
- ### 配置ip及网关
 - pkusz1
    ```bash
    sudo ethtool --offload eth1 gro off
    sudo ifconfig eth1 192.169.1.4
    sudo route add -host 192.169.1.3 dev eth1
    sudo route add -net 192.169.1.0/24 dev eth1 gw 192.169.1.3
    sudo route add -net 192.170.1.0/24 dev eth1 gw 192.169.1.3
    sudo route add -net 192.171.1.0/24 dev eth1 gw 192.169.1.3
    sudo ifconfig eth1 mtu 8400

  - node1
    ```bash
    sudo ethtool --offload enp1s0f3 gro off
    sudo ethtool --offload enp1s0f2 gro off
    sudo ethtool --offload enp1s0f1 gro off
    sudo ethtool --offload enp1s0f0 gro off
    sudo ifconfig enp1s0f0 mtu 8400
    sudo ifconfig enp1s0f1 mtu 8400
    sudo ifconfig enp1s0f2 mtu 8400
    sudo ifconfig enp1s0f3 mtu 8400

     ```
  - node2
    ```bash
    sudo ethtool --offload enp1s0f3 gro off
    sudo ethtool --offload enp1s0f2 gro off
    sudo ethtool --offload enp1s0f1 gro off
    sudo ethtool --offload enp1s0f0 gro off
    sudo ifconfig enp1s0f0 mtu 8400
    sudo ifconfig enp1s0f1 mtu 8400
    sudo ifconfig enp1s0f2 mtu 8400
    sudo ifconfig enp1s0f3 mtu 8400
    
    ```
  - node3
    ```bash
    sudo ethtool --offload enp7s0f3 gro off
    sudo ethtool --offload enp7s0f2 gro off
    sudo ethtool --offload enp7s0f1 gro off
    sudo ethtool --offload enp7s0f0 gro off
    sudo ifconfig enp7s0f3 192.169.1.3
    sudo route add -host 192.169.1.4 dev enp7s0f3
    sudo ifconfig enp7s0f0 mtu 8400
    sudo ifconfig enp7s0f1 mtu 8400
    sudo ifconfig enp7s0f2 mtu 8400
    sudo ifconfig enp7s0f3 mtu 8400
    ```
    
  - node6
    ```bash
    sudo ethtool --offload enp1s0f3 gro off
    sudo ethtool --offload enp1s0f2 gro off
    sudo ethtool --offload enp1s0f1 gro off
    sudo ethtool --offload enp1s0f0 gro off
    sudo ifconfig enp1s0f0 mtu 8400
    sudo ifconfig enp1s0f1 mtu 8400
    sudo ifconfig enp1s0f2 mtu 8400
    sudo ifconfig enp1s0f3 mtu 8400
    
  - node7
    ```bash
    sudo ethtool --offload enp1s0f3 gro off
    sudo ethtool --offload enp1s0f2 gro off
    sudo ethtool --offload enp1s0f1 gro off
    sudo ethtool --offload enp1s0f0 gro off
    sudo ifconfig enp1s0f0 mtu 8400
    sudo ifconfig enp1s0f1 mtu 8400
    sudo ifconfig enp1s0f2 mtu 8400
    sudo ifconfig enp1s0f3 mtu 8400
  
    
  - node8
    ```bash
    sudo ethtool --offload enp1s0f3 gro off
    sudo ethtool --offload enp1s0f2 gro off
    sudo ethtool --offload enp1s0f1 gro off
    sudo ethtool --offload enp1s0f0 gro off
    sudo ifconfig enp1s0f0 mtu 8400
    sudo ifconfig enp1s0f1 mtu 8400
    sudo ifconfig enp1s0f2 mtu 8400
    sudo ifconfig enp1s0f3 mtu 8400
    sudo ifconfig enp1s0f1 192.70.1.3
    sudo route add -host 192.170.1.4 dev enp1s0f1 


  - pkusz3
    ```bash
    sudo ethtool --offload enp1s0f1 gro off
    sudo ifconfig enp1s0f1 192.170.1.4
    sudo route add -host 192.170.1.3 dev enp1s0f1
    sudo ifconfig enp1s0f1 mtu 8400
    sudo route add -net 192.169.1.0/24 dev enp1s0f1 gw 192.170.1.3
    sudo route add -net 192.170.1.0/24 dev enp1s0f1 gw 192.170.1.3
    sudo route add -net 192.171.1.0/24 dev enp1s0f1 gw 192.170.1.3
    

    ```
  

 - node10
    ```bash
    sudo ethtool --offload enp7s0f3 gro off
    sudo ethtool --offload enp7s0f2 gro off
    sudo ethtool --offload enp7s0f1 gro off
    sudo ethtool --offload enp7s0f0 gro off
    sudo ifconfig enp7s0f0 mtu 8400
    sudo ifconfig enp7s0f1 mtu 8400
    sudo ifconfig enp7s0f2 mtu 8400
    sudo ifconfig enp7s0f3 mtu 8400
    sudo ifconfig enp7s0f0 192.171.1.3
    sudo route add -host 192.171.1.4 dev enp7s0f0 
   
  - node9
    ```bash
    sudo ethtool --offload enp7s0f3 gro off
    sudo ethtool --offload enp7s0f2 gro off
    sudo ethtool --offload enp7s0f1 gro off
    sudo ethtool --offload enp7s0f0 gro off
    sudo ifconfig enp7s0f0 mtu 8400
    sudo ifconfig enp7s0f1 mtu 8400
    sudo ifconfig enp7s0f2 mtu 8400
    sudo ifconfig enp7s0f3 mtu 8400
    sudo ifconfig enp7s0f0 192.171.1.4
    sudo route add -host 192.171.1.3 dev enp7s0f0
    sudo route add -net 192.169.1.0/24 dev enp7s0f0 gw 192.171.1.3
    sudo route add -net 192.170.1.0/24 dev enp7s0f0 gw 192.171.1.3
    sudo route add -net 192.171.1.0/24 dev enp7s0f0 gw 192.171.1.3

    ```
  - node11
    ```bash
    sudo ethtool --offload enp1s0f3 gro off
    sudo ethtool --offload enp1s0f2 gro off
    sudo ethtool --offload enp1s0f1 gro off
    sudo ethtool --offload enp1s0f0 gro off
    sudo ifconfig enp1s0f0 mtu 8400
    sudo ifconfig enp1s0f1 mtu 8400
    sudo ifconfig enp1s0f2 mtu 8400
    sudo ifconfig enp1s0f3 mtu 8400
    
    ```
  - node14
    ```bash
    sudo ethtool --offload enp7s0f3 gro off
    sudo ethtool --offload enp7s0f2 gro off
    sudo ethtool --offload enp7s0f1 gro off
    sudo ethtool --offload enp7s0f0 gro off
    sudo ifconfig enp7s0f0 mtu 8400
    sudo ifconfig enp7s0f1 mtu 8400
    sudo ifconfig enp7s0f2 mtu 8400
    sudo ifconfig enp7s0f3 mtu 8400
    ```


- ### 测试
    - 在pkuz1上执行：
        ```bash
        ping 192.169.1.3
        ```
    - 在node1上执行：
        ```bash
        ping 192.169.1.2
        ```
    - 在pkusz2上执行：
        ```bash
        ping 192.169.1.5
    - 在pkusz3上执行：
        ```bash
        ping 192.169.1.8
    - 网络拓扑
    
        ![测试网络拓扑](https://raw.githubusercontent.com/SunnyQjm/ip-ndn-stack_cpp/master/documents/topology.png)
 
