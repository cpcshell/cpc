#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cpcdos/cpcldr.h>
#include <cpcdos/logger.h>
#include <cpcdos/cpcldr/window_manager/wm.h>
#include "daemonize.h"

void
on_exit(void)
{
	wm_stop();
	remove(PID_FILE);

	LOG_DEINIT();
}

int
main(int argc, char const *argv[])
{
	(void)argc;

	if (access(PID_FILE, F_OK) == 0)
	{
		fprintf(stderr, "Already running");
		return (EXIT_FAILURE);
	}
	atexit(on_exit);
	daemonize("/", PID_FILE);
	LOG_INIT(argv[0]);
	LOG(LOG_DEBUG, "Hello world");
	wm_run();
	return (EXIT_SUCCESS);
}