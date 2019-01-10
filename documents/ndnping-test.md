> # 两台NDN主机之间进行ndnping测试连通性
> - **主机A**：
>   - 网卡名：ens3
>   - MAC地址：e0:d5:5e:bb:d8:82
> - **主机B**：
>   - 网卡名：ens2
>   - MAC地址：e0:d5:5e:bb:d8:83
> - 然后，A、B主机的上述两个网口是一根网线直连的

- ## 首先保证A、B主机的nfd进程处于启动状态
    ```bash
    nfd-status

    # 如果未启动，则执行下面的命令启动
    nfd-start
    ```

- ## 分别为A、B主机创建Face
    - 主机A执行：
    ```bash
    nfdc face create remote ether://[e0:d5:5e:bb:d8:83] local dev://[ens2]
    ```
    
    - 主机B执行：
    ```bash
    nfdc face create remote ether://[e0:d5:5e:bb:d8:82] local dev://[ens3]
    ```

- ## 配置一条A到B的路由
    首先你要在主机A上执行 `nfd-status` 查看刚才创建的face的id，将下面命令中的 `<faceId>` 替换成实际的id
    ```bash
    nfdc route add prefix /a nexthop <faceId>
    ```

- ## 进行ndnping测试
    - 在主机B上开启ndnpingserver:
        ```bash
        sudo ndnpingserver /a
        ```
    - 在主机A上ping：
        ```bash
        sudo ndnping /a
        ```
