#ifndef WT_LOGGER_H
#define WT_LOGGER_H

#include <cstdio>

#define LogPrint(level, fmt, ...) \
    fprintf(stderr, "[%s][%s][%s:%d]" fmt "\n", level, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define WtLogErr(fmt, ...) LogPrint("Error", fmt, ##__VA_ARGS__)
#define WtLogInfo(fmt, ...) LogPrint("Info", fmt, ##__VA_ARGS__)
#define WtLogDebug(fmt, ...) LogPrint("Debug", fmt, ##__VA_ARGS__)

#endif // WT_LOGGER_H
