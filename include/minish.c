/**
 * minish.c
 *
 * @Author : lostprobe
 * @Date   : 2023/01/23 18:25
 * @Brief  : minish.c
 */
#include <readline/readline.h>
#include <readline/history.h>

#include <string.h>
#include <stdlib.h>

#include "minish.h"
#include "debug.h"

static char * rl_gets(const char * prompt)
{
    static char * line_read = NULL;
    if(line_read)
    {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline(prompt);

    // 如果 line_read 不为空，则加入 history
    if(line_read && *line_read)
        add_history(line_read);

    return line_read;
}

int minish(cmd_config* config)
{
    char * str, *str_end, *cmd, *args;
    int i;
    while(1)
    {
        str = rl_gets(config->prompt);
        str_end = str + strlen(str);
        cmd = strtok(str, " ");
        if(cmd == NULL)
            continue;
        args = str + strlen(cmd) + 1;
        if(args >= str_end)
            args = NULL;
        for(i = 0; i < config->cmd_table_len; i++)
        {
            if(strcmp(config->cmd_table[i].name, cmd) == 0)
            {
                config->cmd_table[i].handler(args);
                break;
            }
        }
        if(i >= config->cmd_table_len)
            printf(ANSI_FMT("Unknown command `%s`\n", ANSI_FG_RED), cmd);
    }
    return 0;
}