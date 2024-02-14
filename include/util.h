/**
 * util.h
 *
 * @author : lostprobe
 * @date   : 2023/06/16 18:22
 * @brief  : util.h
 */
#ifndef UTIL_H
#define UTIL_H

// 平台无关

/// @brief 返回大于 x 的最小 2 的幂
/// @param x 
/// @return 
int round_two(int x);

/// @brief 可接收格式化字符串的system()函数
/// @param fmt 格式字符串
/// @param ... 格式字符串的参数
/// @return system()函数的返回值
int systemf(const char * fmt, ...);


#endif // UTIL_H