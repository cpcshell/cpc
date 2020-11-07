#include <cstdio>

#include "cpinti/debug.h"

namespace cpinti::debug
{
    static const char *_level_type[] = {
        "trace",
        "info",
        "warn",
        "error",
        "fatal",
    };

    void __log(level l, const char *fmt, va_list args)
    {
        fprintf(stderr, "* %s: ", _level_type[l]);
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");
    }
} // namespace cpinti::debug
