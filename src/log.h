#ifndef LOG_H
#define LOG_H

#include <stdio.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
} LogLevel;

void log_init(const char *filename);
void log_close(void);
void log(LogLevel level, const char *format, ...);

#define log_debug(format, ...) log(LOG_DEBUG, format, __VA_ARGS__)
#define log_info(format, ...) log(LOG_INFO, format, __VA_ARGS__)
#define log_warning(format, ...) log(LOG_WARNING, format, __VA_ARGS__)
#define log_error(format, ...) log(LOG_ERROR, format, __VA_ARGS__)

#endif
