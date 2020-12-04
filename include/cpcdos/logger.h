#ifndef CPCDOS_LOGGER_H
# define CPCDOS_LOGGER_H 1

# include <syslog.h>

# define LOG_INIT(x) openlog(x, LOG_CONS | LOG_PID, LOG_LOCAL7)

# define LOG(lvl, ...) __log(lvl, __FILE__, __FUNCTION__, \
									__LINE__, __VA_ARGS__)

# define LOG_DEINIT closelog

void	__log(int, const char *, const char *, int, const char *, ...);

#endif /* !CPCDOS_LOGGER_H */