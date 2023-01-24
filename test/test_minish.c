#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minish.h"
#include "debug.h"

int cmd_exit(char * args)
{
    exit(0);
}

int cmd_p(char * args)
{
    if(args)
        printf("%s\n", args);
}

int main(int argc, char ** argv)
{
    cmd_config cmd;
    cmd.prompt = "minish> ";
    cmd_entry entry[2] = {
            {"exit", "exit", cmd_exit},
            {"p", "p [msg]", cmd_p}
        };
    cmd.cmd_table_len = 2;
    cmd.cmd_table = entry;
    minish(&cmd);

    return 0;
}