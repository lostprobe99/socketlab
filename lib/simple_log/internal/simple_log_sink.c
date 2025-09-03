/**
 * simple_log_sink.c
 *
 * @author : lostprobe
 * @date   : 2025/09/01 21:35
 * @brief  : simple_log_sink.c
 */

#include "simple_log.h"
#include "simple_log_sink.h"

log_sink_t* slog_get_sink_p()
{
    static log_sink_t sink = {
        .fp = NULL,
        .file = NULL,
        .owned = 0,
    };
    return &sink;
}

FILE* slog_get_sink_fp()
{
    log_sink_t* sink = slog_get_sink_p();
    return sink->fp;
}

char * slog_get_sink_file()
{
    log_sink_t* sink = slog_get_sink_p();
    return sink->file;
}

int slog_get_sink_owned()
{
    log_sink_t* sink = slog_get_sink_p();
    return sink->owned;
}

void slog_set_sink_fp(FILE* fp)
{
    log_sink_t* sink = slog_get_sink_p();
    if( sink->owned )
    {
        if( sink->fp )
        {
            fclose(sink->fp);
            sink->fp = NULL;
        }
    }
    sink->fp = fp;
}

void slog_set_sink_file(const char *file)
{
    log_sink_t* sink = slog_get_sink_p();
    if( sink->file )
    {
        free(sink->file);
        sink->file = NULL;
    }
    if( file == NULL)
        sink->file = NULL;
    else
        sink->file = strdup(file);
}

void slog_set_sink_owned(int owned)
{
    log_sink_t* sink = slog_get_sink_p();
    sink->owned = owned;
}

void slog_set_sink(FILE* fp, const char *file, int owned)
{
    slog_set_sink_fp(fp);
    slog_set_sink_file(file);
    slog_set_sink_owned(owned);
}