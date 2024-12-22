#include "philosopher.h"
#include "philo.h"

long long   get_timestamp(void)
{
    struct timeval  tv;
    long long       milliseconds;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return (milliseconds);
}

long long   time_since_start(t_data *data)
{
    return (get_timestamp() - data->start_time);
}

void    precise_sleep(long long time_in_ms, t_data *data)
{
    long long start;

    start = get_timestamp();
    while (!(data->someone_died))
    {
        if ((get_timestamp() - start) >= time_in_ms)
            break;
        usleep(100);
    }
}

void    print_action(t_data *data, int id, char *action)
{
    pthread_mutex_lock(&data->writing);
    if (!data->someone_died)
        printf("%lld %d %s\n", time_since_start(data), id, action);
    pthread_mutex_unlock(&data->writing);
}
