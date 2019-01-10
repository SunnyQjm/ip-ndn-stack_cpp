# ip-ndn-stack_cpp
> ## 最小原型网络环境配置


- [pkun3-pkun1-pkun2-hitn1](https://github.com/SunnyQjm/ip-ndn-stack_cpp/tree/master/documents/pkun3-pkun1-pkun2-hitn1.md)
- [node1-node2-node3-pkusz1](https://github.com/SunnyQjm/ip-ndn-stack_cpp/tree/master/documents/node1-node2-node3-pkusz1.md)


- ### 配置nfd静态路由
    > 静态路由的配置，可以像下面这样手动配置，也可以使用协助脚本，详情参见[IP_NDN_STACK 部署脚本](https://github.com/SunnyQjm/ip-ndn-stack_cpp/tree/master/deployment)
    
  - pkun1
    ```bash
    sudo nfdc route add prefix /IP/pre/192.169.1.4 nexthop <faceid>
    sudo nfdc route add prefix /IP/192.169.1.4 nexthop <faceid>
    sudo nfdc route add prefix /IP/TCP/pre/192.169.1.4 nexthop <faceid>
    sudo nfdc route add prefix /IP/TCP/192.169.1.4 nexthop <faceid>
    ```
  - pkun2
    ```bash
    sudo nfdc route add prefix /IP/pre/192.169.1.1 nexthop <faceid>
    sudo nfdc route add prefix /IP/192.169.1.1 nexthop <faceid>
    sudo nfdc route add prefix /IP/TCP/pre/192.169.1.1 nexthop <faceid>
    sudo nfdc route add prefix /IP/TCP/192.169.1.1 nexthop <faceid>
    ```

> ## 编译项目

- ### 首先用cmake生成Makefile
    进入项目根目录，然后执行下面命令(如果没有安装cmake，请自行安装)
    ```bash
    cmake .
    ```
- ### 然后编译项目
    ```bash
    make
    ```
- ### 运行
    ```bash
    sudo ./ip_ndn_stack_cpp <config file path>

    # for example
    sudo ./ip_ndn_stack_cpp ./config/part1.json
    ```

> ## 参与项目
> - 要对项目做出贡献，即贡献代码，请fork一份到自己的仓库进行操作。具体操作方式可以参考[Git-对项目做出贡献](https://git-scm.com/book/zh/v2/GitHub-%E5%AF%B9%E9%A1%B9%E7%9B%AE%E5%81%9A%E5%87%BA%E8%B4%A1%E7%8C%AE)

- ### 拉取项目到本地
  ```bash
  git clone https://github.com/SunnyQjm/ip-ndn-stack_cpp.git
  ```

- ### 从github上拉取最新的代码
  首先，进入项目的根目录
  ```bash
  cd ip-ndn-stack_cpp
  ```
  然后执行git pull
  ```bash
  git pull
  ```

- ### 添加了新代码，要推送到github上
  ```bash
  git add <filename>
  
  git commit -m <这里填写更新说明>
  
  git push 
  ```
