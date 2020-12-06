#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <czmq.h>
#include <cpcdos/cpcdos.h>
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
	setlocale(LC_ALL, "");
	bindtextdomain("cpcdos", getenv("PWD"));
	textdomain("cpcdos");

	if (access(PID_FILE, F_OK) == 0)
	{
		fprintf(stderr, _("%s is already running"), argv[0]);
		return (EXIT_FAILURE);
	}
	atexit(on_exit);
	daemonize("/", PID_FILE);
	LOG_INIT(argv[0]);
	LOG(LOG_DEBUG, _("Hello world"));

	ipc = zsock_new_rep(IPC_FILE);
	if (ipc == NULL)
	{
		LOG(LOG_ERR, _("Failed to initialize ipc"));
		return (EXIT_FAILURE);
	}
	zsock_destroy(&ipc);
	wm_run();
	return (EXIT_SUCCESS);
}