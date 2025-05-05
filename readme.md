# socketlab

## 介绍

socket编程相关

## changelog:
* 2025-05-05
    1. 添加 `netpackets` 库
    2. 拆分 `include` 下的 `arp`, `ether` 到 `lib/netpackets`
    3. 其他连带调整，保证编译通过
    4. 修复 `os_exec` 中函数 `execvp` 参数错误
    5. [nl_get_ipaddr.c](src/nl_get_ipaddr.c) 修正 `netlink socket` 创建和绑定逻辑

* 2025-05-02
    1. 添加 `simple_log`
    2. 添加 `netlink` 获取 `IP` 地址的示例

* 2025-02-22
    1. 删除`send_ether_frame`中的未使用参数
    2. 修改[src/arp_scan.c](src/arp_scan.c)中的 `ARP` 包过滤逻辑

* 2025-1-4
    1. 添加 `extern C`, 适配 c++
    2. 删除 `buffer_t`
    3. [src/arp_scan.c](src/arp_scan.c) 添加 `ARP` 报文过滤函数
    4. 将 `ARP` 部分从 `util` 中拆分到 `arp` 中
    5. 将 `ether` 部分从 `util` 中拆分到 `ether` 中
    6. 移除 `util` 中的 `systemf`, 添加 `os_exec`
    7. 添加 `exec` 函数族的示例和解释

* 2024-12-8
    1. debug 宏调整，优化实现和打印格式
    2. 将 `arp_scan` 中的 `bind_itf` 函数拆分到 `util` 中
    3. `util` 中添加 `mac_aton` 和 `mac_ntoa` 用于 MAC 格式转换
    4. 添加 [src/send_ether.c](src/send_ether.c)，实现以太网帧的打包发送

* 2024-12-1
    1. 重命名 `FATAL` 为 `DIE`，以更准确地反映其功能，errif 为 `DIE_IF`

* 2024-11-23 ~ 2024-11-30
    1. 新增 [src/arp_scan.c](src/arp_scan.c) 文件实现 ARP 扫描功能
    2. 在 common.c 中添加设置接收和发送超时的函数
    3. `arping`函数调整，将创建 `socket` 到关闭 `socket`的部分拆分到 `arping1`，`arping1` 中调用 `arping` 实现独立的 `ARP` 发送流程

* 2024-11-23
    1. 将 `arping` 中填充 `arp_msg_t` 的部分拆分为函数 `pack_ether_hdr` 和 `pack_arp_msg`

* 2024-9-1 ~ 2024-11-10
    1. 修复`systemf`中的错误
    2. 添加一个时间戳获取函数`timestamp`
    3. 添加 `hexdump`, `hexdump1` 和 `reverse32` 函数
    4. 添加 `get_itf_mac`, `get_itf_ip4` 和 `get_itf_subnet_mask` 函数
    5. 添加 `arping` 函数
    6. 添加 [`arping` demo](src/arping.c)
    7. 添加 `set_broadcast` 函数
    8. 添加 `broadcast` [收](src/broadcast_listen.c)[发](src/broadcast_send.c)的demo
    9. 添加 [`popen` demo](test/popen.c)
    10. 删除 `buffer_t`

* 2024-9-1
    1. 添加获取网络接口的示例[src/get_itfs](src/get_itfs.c)

* 2024-7-7
    1. 添加守护进程示例[src/daemon.c](src/daemon.c)

* 2024-2-18
    1. 添加`TcpSocket`实现的多线程echo服务端[src/tcpsocket-server.cpp](src/tcpsocket-server.cpp)

* 2024-2-16
    1. 添加udp实现的echo客户端[src/uecho-client.c](src/uecho-client.c)和服务器[src/uecho-server.c](src/uecho-server.c)
    2. 为debug宏`FATAL`和`WARN`添加换行

* 2024-2-14
    1. `util`中增加`systemf`函数
    2. 修改`Socket`类，将`Socket`作为基类，添加子类`TcpSocket`

* 2024-1-20
    1. 为`InetAddr`添加了一个af函数
    2. 修改了`InetAddr::addr`函数中`inet_addr`的使用错误

* 2024-1-7
    1. 移除`Socket`的拷贝构造函数和拷贝赋值函数，添加移动构造函数和移动赋值函数
    2. 将`Socket`的socket关闭操作拆分为单独的函数

* 2024-1-7
    1. 调整目录结构，将`Socket`和`InetAddr`拆分到`lib/simplesocket`

* 2023-6-19
    1. 为`Socket`添加一个`shared_ptr`作为引用计数来控制复制和析构
    2. 使用`Socket`和`InetAddr`实现一个回声服务器[src/obj-echo-server.c](src/obj-echo-server.cpp)

* 2023-6-17
    增加一个epoll实现的回声服务器[src/epoll-echo-server.c](src/epoll-echo-server.c)

* 2023-6-16
    1. 增加了一个`EXPECT`宏
    2. 增加了一个`round_two`，用于取大于 n 的最小 2 的幂
    3. 增加了一个`buffer_make_s`，用于通过字符串创建`buffer`
    4. 增加了一个`fileno`和`fdopen`的示例

* 2023-6-15
    将根目录的`CMakeLists.txt`拆分为子目录

* 2023-6-15
    添加changelog

* 2023-6-7
    添加一个`writev`函数的示例[src/writev.c](src/writev.c)

* 2023-6-4
    1. 添加select监听stdin的示例
    2. 添加select实现的回声服务器[src/select-echo-server.c](src/select-echo-server.c)
    3. 在[common.h](include/common.h)中添加一个宏`AUTOFREE`，可自动释放malloc内存，依赖于GNUC的`attribute cleanup`特性
    4. 在[CMakeLists.txt](./CMakeLists.txt)中定义操作系统宏和编译器宏
    5. 添加一个`buffer`结构体

* 2023-6-2
    整理目录结构

* 2023-3-21
    1. 封装socket为类，未测试
    2. 添加标准库中的thread库的示例[test/test_thread.c](test/test_thread.c)

* 2023-2-20
    修复`common`的`server`中的错误

* 2023-2-12
    1. 在`common`中封装了服务端函数`server`，客户端函数`c_connect`，判断tcp连接的函数`tcp_connected`，获取tcp信息的函数`get_tcp_info`
    2. 在[common.h](include/common.h)中添加了doxygen格式的注释
    3. 添加了[src/mp-cs.c](src/mp-cs.c)，fork出两个子线程作为服务端和客户端

* 2023-2-2
    1. 在[include/common.c](include/common.c)的`make_sockaddr`中添加域名解析，并且当地址为空时使用`INADDR_ANY`
    2. 将文件传输程序服务端改为多线程

* 2023-1-29
    1. 在文件传输程序的`get`中添加目标文件的访问检查，使当文件不可读时返回错误信息
    2. 实现`put`并通过md5校验

* 2023-1-28
    修复文件传输程序的`get`，md5校验成功

* 2023-1-25
    实现文件传输程序的`cd`和`get`，

* 2023-1-24
    1. 实现了一个简易的shell [include/minish.h](include/minish.h), [include/minish.c](include/minish.c), 需要posix readline库
    2. 基于minish实现了一个新的文件传输程序[src/ft/*](src/ft)，实现了`ls`, `help`, `pwd`, `open`命令

* 2023-1-21
    1. 分别为win和linux重新定义socket相关类型
    2. 在[common.h](include/common.h)中添加了创建socket的函数`make_socket`和创建sockaddr_in的函数`make_sockaddr`
    3. 添加[include/debug.h](include/debug.h)

* 2022-4-12
    添加多进程的回声服务器[src/mp-echo-server.c](src/mp-echo-server.c)

* 2022-3-10
    1. 添加回声服务器和客户端[src/echo-client.c](src/echo-client.c), [src/echo-server.c](src/echo-server.c)
    2. 添加四则运算程序[src/op-client.c](src/op-client.c), [src/op-server.c](src/op-server.c)
    3. 添加信号处理示例[src/signal.c](src/signal.c)

* 2022-3-1 
    1. 添加一个使用`getsockopt`获取socket类型的示例[src/sock-type.c](src/sock-type.c)
    2. 添加一个使用`getsockopt`获取socket缓冲区大小的示例[src/get-buf.c](src/get-buf.c)

* 2022-2-25及以前 
    1. 添加一个简易的文件传输程序[src/ft-client.c](src/ft-client.c), [src/ft-server.c](src/ft-server.c)
    2. 添加一个公用头文件[include/common.h](include/common.h)
