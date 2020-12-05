#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

void
cpcdos_log(int lvl, const char *file, const char *func, int line,
											const char *msg, ...)
{
	int n;
	int size;
	char *buff;
	char *tmp;
	va_list ap;

	size = 512;
	buff = (char *)malloc(size);
	if (buff == NULL)
		return;
	
	do
	{
		va_start(ap, msg);
		n = vsnprintf(buff, size, msg, ap);
		va_end(ap);

		if (n < 0)
			return;
		
		if (n < size)
			break;
		
		size = n + 1;
		tmp = (char *)realloc(buff, size);
		if (tmp == NULL)
		{
			free(buff);
			return;
		}
		buff = tmp;
	}
	while (1);

	syslog(lvl, "[%s:%s:%d] %s", file, func, line, buff);
	free(buff);
}