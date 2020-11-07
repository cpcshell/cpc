#ifndef CPCDOS_DEBUG
#define CPCDOS_DEBUG

#include <cstdarg>
#include <cstdlib>

namespace cpinti::debug
{
    enum level
    {
        TRACE,
        INFO,
        WARN,
        ERROR,
        FATAL,
    };

    void __log(level, const char *, va_list args);

    inline void trace(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);

        __log(TRACE, fmt, ap);

        va_end(ap);
    }

    inline void info(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);

        __log(INFO, fmt, ap);

        va_end(ap);
    }

    inline void warn(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);

        __log(WARN, fmt, ap);

        va_end(ap);
    }

    inline void error(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);

        __log(ERROR, fmt, ap);

        va_end(ap);
    }

    inline void fatal(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);

        __log(FATAL, fmt, ap);

        va_end(ap);

        _Exit(-1);
    }

} // namespace cpinti::debug

#endif /* CPCDOS_DEBUG */
