/**
 * simple_log_sink.h
 *
 * @author : lostprobe
 * @date   : 2025/09/01 21:35
 * @brief  : simple_log_sink.h
 */
#ifndef SIMPLE_LOG_SINK_H
#define SIMPLE_LOG_SINK_H

#include "simple_log.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct log_sink_s {
    FILE *fp;
    char *file;
    int owned;
} log_sink_t;

log_sink_t* slog_get_sink_p();
FILE* slog_get_sink_fp();
char * slog_get_sink_file();
int slog_get_sink_owned();
void slog_set_sink_fp(FILE* fp);
void slog_set_sink_file(const char *file);
void slog_set_sink_owned(int owned);
void slog_set_sink(FILE* fp, const char *file, int owned);

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LOG_SINK_H