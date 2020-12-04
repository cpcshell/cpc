#include <cpcdos/logger.h>

int main(int argc, char const *argv[])
{
    (void)argc;

    LOG_INIT(argv[0]);
	LOG(LOG_DEBUG, "Hello world");
    LOG_DEINIT();
	return (0);
}