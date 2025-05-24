#!/usr/bin/env bash

# test_send_ether.sh

# Check if enough arguments are provided
if [ $# -lt 4 ]; then
    echo "Usage: $0 <interface> <destination_mac> <protocol> <data>"
    exit 1
fi

# 示例命令: sudo test/test_send_ether.sh eth0 00:11:22:33:44:55 800 "Hello, World"

# 获取当前目录并定位到 build 目录
CURRENT_DIR=$(cd "$(dirname "")" && pwd)
BUILD_DIR=${CURRENT_DIR}/build
# TEST_DIR=${CURRENT_DIR}/test
LOG_DIR=${CURRENT_DIR}/log
echo "current dir is: ${CURRENT_DIR}"
echo "build   dir is: ${BUILD_DIR}"
# echo "test    dir is: ${TEST_DIR}"
echo "log     dir is: ${LOG_DIR}"

# 检查文件是否存在
if [ ! -f "${BUILD_DIR}/src/send_ether" ]; then
    echo "请编译程序后再运行测试用例"
    exit 1
fi

# 使用tcpdump来验证数据是否真的被发送（需要root权限）
if command -v tcpdump &> /dev/null; then
    echo -e "正在使用tcpdump验证数据传输...\r"
    # -i: 指定网卡
    # -c: 抓取指定数量的数据包
    # ether dst $2: 过滤目标 MAC 地址
    # -XXX: 以 16 进制显示完整的链路层数据, 并尝试解析上层协议
    # -e: 打印链路层头部
    sudo tcpdump -i $1 -c 1 ether dst $2 -XXX -e 2>&1 > ${LOG_DIR}/tcpdump.out &
    TCPDUMP_PID=$!
    sleep 2

    echo -e "启动程序: send_ether $1 $2 $3 $4\r"
    # Run the program with provided arguments
    ${BUILD_DIR}/src/send_ether "$1" "$2" "$3" "$4" > /dev/null 2>&1 
    RESULT=$?

    # 验证程序是否成功执行
    if [ $RESULT -eq 0 ]; then
        echo -e "[PASS] 程序成功发送以太网帧\r"
    else
        echo -e "[FAIL] 程序发送以太网帧失败，返回码: $RESULT\r"
        exit 1
    fi

    sleep 2 # 给tcpdump一点时间来捕获数据包
    
    # 杀死tcpdump进程
    # kill $TCPDUMP_PID > /dev/null 2>&1
    
    # 检查是否有数据包被捕获
    if cat ${LOG_DIR}/tcpdump.out | grep -q 00:11:22:33:44:55 > /dev/null 2>&1; then
        echo -e "[PASS] tcpdump验证通过，检测到数据包发送\r"
    else
        echo -e "[FAIL] tcpdump未检测到数据包发送\r"
    fi
else
    echo "提示: 安装tcpdump可以获得更详细的网络数据包验证"
fi