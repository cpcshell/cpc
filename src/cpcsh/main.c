#include <stdlib.h>
#include <cpcdos/cpcdos.h>
#include <cpcdos/logger.h>

int main(int argc, char const *argv[])
{
	(void)argc;
	setlocale(LC_ALL, "");
	bindtextdomain("cpcdos", getenv("PWD"));
	textdomain("cpcdos");

	LOG_INIT(argv[0]);
	LOG(LOG_DEBUG, _("Hello world"));
	LOG_DEINIT();
	return (0);
}