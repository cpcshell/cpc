#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <czmq.h>
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
	zsock_t *ipc;

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

	ipc = zsock_new_rep(IPC_FILE);
	if (ipc == NULL)
	{
		LOG(LOG_ERR, "Failed to initialize ipc");
		return (EXIT_FAILURE);
	}
	zsock_destroy(&ipc);
	wm_run();
	return (EXIT_SUCCESS);
}