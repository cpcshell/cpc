#include <pthread.h>

#include "compat/dos.h"

static pthread_mutex_t mutex_stock = PTHREAD_MUTEX_INITIALIZER;

void enable(void)
{
    pthread_mutex_lock(&mutex_stock);
}

void disable(void)
{
    pthread_mutex_unlock(&mutex_stock);
}