#include "log.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>

static FILE *log_file = NULL;

void log_init(const char *path) {
    log_file = fopen(path, "w");
    if (!log_file) {
        fprintf(stderr, "Failed to open log file: %s\n", path);
    }
}

void log_close(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log(LogLevel level, const char *fmt, ...) {
    if (!log_file) return;

    const char *level_str[] = { "DEBUG", "INFO", "WARN", "ERROR" };
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm);

    fprintf(log_file, "[%s] [%s] ", timestamp, level_str[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);  // Always flush to avoid missing logs on crash
}

