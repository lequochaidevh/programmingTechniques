#ifndef __debug_macro__
#define __debug_macro__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>// Cần cho việc lấy time

//
// Hàm hiển thị log có thêm nhãn thời gian
//
void time_printf(const char *fmt, ... )
{
    // Tính thời gian hiện tại
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf (stderr, "[%d-%d-%d_%d:%d:%d] ", timeinfo->tm_year + 1900,timeinfo->tm_mon + 1, timeinfo->tm_mday,  timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    // Hiển thị log như cũ
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

#ifdef NDEBUG
#define log_debug(fmt, ...)  do {} while (0)
#define log_err(fmt, ...)  do {} while (0)
#define log_warn(fmt, ...)  do {} while (0)
#define log_info(fmt, ...)  do {} while (0)
#else // NDEBUG

#define log_debug(fmt, ...) \
        do { time_printf("[DEBUG] %10s:%10d:%15s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)

#define log_err(fmt, ...) \
        do { time_printf("[ERROR] %10s:%10d:%15s(): " fmt, __FILE__,  __LINE__, __func__,## __VA_ARGS__); } while (0)

#define log_warn(fmt, ...) \
        do { time_printf("[WARN] %10s:%10d:%15s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)

#define log_info(fmt, ...) \
        do { time_printf("[INFO] %10s:%10d:%15s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)
#endif //NDEBUG

#endif // __debug_macro__

