/**
 * minish.h
 *
 * @author : lostprobe
 * @date   : 2023/01/23 18:25
 * @brief  : minish.h
 */

#ifndef MINISH_H
#define MINISH_H

/// @brief  描述一个 minish 命令
typedef struct {
    /// @brief 命令名称
    const char * name;
    /// @brief 命令描述文本
    const char * desc;
    /// @brief 命令对应的处理程序
    int (*handler) (char *);
} cmd_entry;

/// @brief minish 配置
typedef struct {
    /// @brief minish 提示符
    const char * prompt;
    /// @brief cmd_entry 数组，保存 minish 命令
    cmd_entry* cmd_table;
    /// @brief cmd_table 长度
    int cmd_table_len;
} cmd_config;

/// @brief 启动一个 minish
/// @param  minish 的配置参数
/// @return 状态码
int minish(cmd_config *);

#endif // MINISH_H