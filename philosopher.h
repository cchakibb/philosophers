#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <pthread.h>
#include "philo.h"

typedef struct s_philosopher
{
    int                 id;
    pthread_t           thread;
    pthread_mutex_t     *left_fork;
    pthread_mutex_t     *right_fork;
    long long           last_meal;
    int                 meals_eaten;
    struct s_data       *data;
}               t_philosopher;

void    *philosopher_routine(void *arg);

#endif
