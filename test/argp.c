/**
 * argp.c
 *
 * @author : lostprobe
 * @date   : 2025/05/24 14:11
 * @brief  : argp.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <argz.h>

#define ARG_NUM 4

/* argp 全局变量 */
/* 程序版本信息, 配置后添加一个 --version 和 -V 选项 */
const char *argp_program_version = "v0.1.0";
/* 配置后将在 --help 结尾显示 */
const char *argp_program_bug_address = "<someone@addr.com>";

struct arguments{
    char *argz;
    size_t argz_len;
    int arg_count;
};

/*
 * @param key 对应 options 中的 key
 * @param arg 对应 options 中的 arg
 * @param state
*/
static int parse_opt(int key, char * arg, struct argp_state * state)
{
    struct arguments *a= state->input;
    switch (key)
    {
    case 'd':
        for(int i = 0; i < (arg ? atoi(arg) : 1); i++)
        {
            printf("d");
        }
        // printf("\n");
        break;
    case 777:
        /* 显示调用回调函数来执行其他选项 */
        return parse_opt('d', "3", state);
    case 888:
        printf("-");
        break;
    case ARGP_KEY_ARG:
        // 提取参数
        (*a).arg_count--;
        // printf("arg: %s\n", arg);
        argz_add(&a->argz, &a->argz_len, arg);
        break;
    case ARGP_KEY_INIT:
        // 参数解析前
        a->argz = 0;
        a->argz_len = 0;
        break;
    case ARGP_KEY_END:
        // 参数解析完成后
        if (a->arg_count < 0)
        {
            // 小于 0 表示在 ARGP_KEY_ARG 中减的超过 ARG_NUM 了
            argp_failure(state, 1, 0, "Too many arguments");
        }
        else if(a->arg_count >/*=*/ ARG_NUM)
        {
            argp_failure(state, 1, 0, "Too few arguments");
        }
        // printf("\n");
        break;
    default:
        break;
    }
    return 0;
}

int main(int argc, char ** argv)
{
    struct argp_option options[] = {
        /* argp 自带 help 和 usage 选项 */
        {
            "dot",  // name: 选项名
            'd',    // key: 对应的短选项
            "NUM",  // arg: 选项的参数
            OPTION_ARG_OPTIONAL,    // flags: 该值表示选项的参数为可选, 配置该值后, parse_opt 的 arg 可为 NULL
            "Show some d on the screen" // 选项的描述
        },
        {
            "period",
            'p',
            "PER",
            OPTION_ALIAS,   // 本选项作为前一个选项的别名, 除 name 和 key 外, 其他和前一个选项相同
            "Show some p on the screen"
        },
        {
            "ellipsis", // 配置为等于 -d 3
            777,    // key=777 不可打印字符, 表示没有短选项
            0,
            OPTION_HIDDEN,  // 隐藏选项, 不显示在 help 信息中
            "Show an ellipsis on the screen",
        },
        {
            "dash",
            888,
            0,
            0,
            "Show a dash on the screen",
        },
        { 0 }
    };
    struct argp argp = {
        options,
        parse_opt,
        "WORD [WORD [WORD [WORD]]]"
    };
    struct arguments arguments = {0};
    arguments.arg_count = ARG_NUM;

    // 第六个参数为回调函数传数据
    if(argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0)
    {
        const char *prev = NULL;
        char * word;
        while((word = argz_next(arguments.argz, arguments.argz_len, prev)))
        {
            printf("%s ", word);
            prev = word;
        }
        printf("\n");
        free(arguments.argz);
    }

    return 0;
}