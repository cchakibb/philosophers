#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct s_data
{
    int                 number_of_philosophers;
    long long           time_to_die;
    long long           time_to_eat;
    long long           time_to_sleep;
    int                 number_of_times_each_philosopher_must_eat;
    int                 all_philosophers_must_eat;
    int                 someone_died;
    long long           start_time;
    pthread_mutex_t     *forks;
    pthread_mutex_t     writing;
    t_philosopher       *philosophers;
}               t_data;

#endif
