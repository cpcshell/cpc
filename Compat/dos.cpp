#include <pthread.h>

#include "dos.h"

static pthread_mutex_t mutex_stock = PTHREAD_MUTEX_INITIALIZER;

int enable(void)
{
    pthread_mutex_lock(&mutex_stock);
}

int disable(void)
{
    pthread_mutex_unlock(&mutex_stock);
}