#ifndef FT_H
#define FT_H

enum ft_request_type {
    LS = 1,
    PWD,
    CD,
    GET,
    PUT,
    EXIT,
    BR,
    END = 127   // 表示一个响应的结束
};

char* ft_request_str[] = {
    "",
    "LS",
    "PWD",
    "CD",
    "GET",
    "PUT",
    "EXIT"
};

#endif // FT_H