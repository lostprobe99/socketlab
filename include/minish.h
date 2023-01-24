/**
 * minish.h
 *
 * @Author : lostprobe
 * @Date   : 2023/01/23 18:25
 * @Brief  : minish.h
 */

#ifndef MINISH_H
#define MINISH_H

typedef struct {
    const char * name;
    const char * desc;
    int (*handler) (char *);
} cmd_entry;

typedef struct {
    const char * prompt;
    cmd_entry* cmd_table;
    int cmd_table_len;
} cmd_config;

int minish(cmd_config *);

#endif // MINISH_H