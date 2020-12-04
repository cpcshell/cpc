#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void
daemonize(const char *root, const char *pid_path)
{
	pid_t pid;
	FILE *pid_file;

	pid = fork();

	if (pid < 0)
		exit(EXIT_FAILURE);
	
	if (pid > 0)
		exit(EXIT_SUCCESS);
	
	if (setsid() < 0)
		exit(EXIT_FAILURE);
	
	pid  = fork();

	if (pid < 0)
		exit(EXIT_FAILURE);
	
	if (pid > 0)
		exit(EXIT_SUCCESS);
	
	umask(0);
	chdir(root);

	pid_file = fopen(pid_path, "w");
	if (pid_file != NULL)
	{
		fprintf(pid_file, "%d", getpid());
		fclose(pid_file);
	}
}