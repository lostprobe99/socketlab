#!/usr/bin/python3
"""
 random_gen.py
 
 @author : 通义灵码
 @modied : lostprobe 2025/05/10
 @date   : 2025/05/10 17:03
 @brief  : random_gen.py
"""
import random
import sys

# 显示帮助信息的函数
def show_help():
    print('''
用法: random_gen.py [数量] [分隔符] [语言格式]

参数说明:
  数量       生成的随机数数量 (默认: 32)
  分隔符     数值之间的分隔符 (默认: 空格)
  语言格式   输出格式 (c= C语言数组, py=Python列表, 其他=原始格式)

示例:
  python3 random_gen.py 16 , c    # 生成C语言数组格式
  python3 random_gen.py 8 ' ' py   # 生成Python列表格式
  python3 random_gen.py           # 使用默认参数
''')
    sys.exit(0)

# 解析命令行参数
# if len(sys.argv) == 1:
if len(sys.argv) > 1 and (sys.argv[1] == '-h' or sys.argv[1] == '--help'):
    show_help()

try:
    count = int(sys.argv[1]) if len(sys.argv) > 1 else 32
    delimiter = sys.argv[2] if len(sys.argv) > 2 else ' '
    lang_format = sys.argv[3] if len(sys.argv) > 3 else 'none'
    indent = 4
except ValueError:
    print("参数必须为整数，使用默认值32")
    count = 32
    delimiter = ' '
    lang_format = 'none'

# 生成随机数列表
randoms = ["0x" + f"{random.randint(0, 255):02x}" for _ in range(count)]

# 不同语言格式的数组模板
if lang_format == 'c':
    prefix, suffix, group_size = "uint8_t arr[] = {", "};", 8
elif lang_format == 'py':
    prefix, suffix, group_size = "[", "]", 4
else:
    prefix, suffix, group_size = "", "", 8
    indent = 0

# 按格式输出
print(prefix)
for i in range(0, count, group_size):
    chunk = randoms[i:i+group_size]
    print(' ' * indent + delimiter.join(chunk) + ("," if lang_format == 'c' else ""))
print(suffix)
