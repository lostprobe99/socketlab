# socketlab

## 介绍

socket编程相关

## changelog:

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
