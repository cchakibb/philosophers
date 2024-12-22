#include "philo.h"
#include "philosopher.h"

int initialize_data(t_data *data, int argc, char **argv)
{
    data->number_of_philosophers = atoi(argv[1]);
    data->time_to_die = atol(argv[2]);
    data->time_to_eat = atol(argv[3]);
    data->time_to_sleep = atol(argv[4]);
    data->all_philosophers_must_eat = 0;
    data->someone_died = 0;
    if (argc == 6)
    {
        data->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
        data->all_philosophers_must_eat = 1;
    }
    else
        data->number_of_times_each_philosopher_must_eat = -1;
    return (0);
}

int initialize_mutexes(t_data *data)
{
    int i;

    data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
    if (!data->forks)
        return (1);
    i = 0;
    while (i < data->number_of_philosophers)
    {
        if (pthread_mutex_init(&data->forks[i], NULL))
            return (1);
        i++;
    }
    if (pthread_mutex_init(&data->writing, NULL))
        return (1);
    return (0);
}

int initialize_philosophers(t_data *data)
{
    int i;

    data->philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
    if (!data->philosophers)
        return (1);
    i = 0;
    while (i < data->number_of_philosophers)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].left_fork = &data->forks[i];
        data->philosophers[i].right_fork = &data->forks[(i + 1) % data->number_of_philosophers];
        data->philosophers[i].last_meal = 0;
        data->philosophers[i].meals_eaten = 0;
        data->philosophers[i].data = data;
        i++;
    }
    return (0);
}

void    *monitor_routine(void *arg)
{
    t_data  *data;
    int     i;

    data = (t_data *)arg;
    while (!data->someone_died)
    {
        i = 0;
        while (i < data->number_of_philosophers && !data->someone_died)
        {
            pthread_mutex_lock(&data->writing);
            if ((get_timestamp() - data->philosophers[i].last_meal) > data->time_to_die)
            {
                print_action(data, data->philosophers[i].id, "died");
                data->someone_died = 1;
            }
            pthread_mutex_unlock(&data->writing);
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}

#include "philo.h"
#include "philosopher.h"

int create_threads(t_data *data)
{
    int         i;
    pthread_t   monitor;

    data->start_time = get_timestamp();
    i = 0;
    while (i < data->number_of_philosophers)
    {
        data->philosophers[i].last_meal = data->start_time;
        if (pthread_create(&data->philosophers[i].thread, NULL, philosopher_routine, &data->philosophers[i]))
            return (1);
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_routine, data))
        return (1);
    pthread_join(monitor, NULL);
    i = 0;
    while (i < data->number_of_philosophers)
    {
        pthread_join(data->philosophers[i].thread, NULL);
        i++;
    }
    return (0);
}

void    *monitor_routine(void *arg)
{
    t_data  *data;
    int     i;
    int     all_eaten;

    data = (t_data *)arg;
    while (!data->someone_died)
    {
        i = 0;
        all_eaten = 1;
        while (i < data->number_of_philosophers && !data->someone_died)
        {
            pthread_mutex_lock(&data->writing);
            if ((get_timestamp() - data->philosophers[i].last_meal) > data->time_to_die)
            {
                print_action(data, data->philosophers[i].id, "died");
                data->someone_died = 1;
            }
            if (data->all_philosophers_must_eat && data->philosophers[i].meals_eaten < data->number_of_times_each_philosopher_must_eat)
                all_eaten = 0;
            pthread_mutex_unlock(&data->writing);
            i++;
        }
        if (data->all_philosophers_must_eat && all_eaten)
        {
            data->someone_died = 1;
            break;
        }
        usleep(1000);
    }
    return (NULL);
}

int main(int argc, char **argv)
{
    t_data  data;

    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
    }
    if (initialize_data(&data, argc, argv))
    {
        printf("Initialization error\n");
        return (1);
    }
    if (initialize_mutexes(&data))
    {
        printf("Mutex initialization error\n");
        return (1);
    }
    if (initialize_philosophers(&data))
    {
        printf("Philosopher initialization error\n");
        return (1);
    }
    if (create_threads(&data))
    {
        printf("Thread creation error\n");
        return (1);
    }
    // Nettoyage des ressources
    pthread_mutex_destroy(&data.writing);
    for (int i = 0; i < data.number_of_philosophers; i++)
        pthread_mutex_destroy(&data.forks[i]);
    free(data.forks);
    free(data.philosophers);
    return (0);
}
