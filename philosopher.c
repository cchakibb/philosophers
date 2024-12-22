#include "philosopher.h"
#include "philo.h"

void    take_forks(t_philosopher *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_action(philo->data, philo->id, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);
    print_action(philo->data, philo->id, "has taken a fork");
}

void    eat(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->writing);
    philo->last_meal = get_timestamp();
    pthread_mutex_unlock(&philo->data->writing);
    print_action(philo->data, philo->id, "is eating");
    precise_sleep(philo->data->time_to_eat, philo->data);
    pthread_mutex_lock(&philo->data->writing);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->writing);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void    sleep_and_think(t_philosopher *philo)
{
    print_action(philo->data, philo->id, "is sleeping");
    precise_sleep(philo->data->time_to_sleep, philo->data);
    print_action(philo->data, philo->id, "is thinking");
}

void    *philosopher_routine(void *arg)
{
    t_philosopher   *philo;

    philo = (t_philosopher *)arg;
    if (philo->id % 2 == 0)
        usleep(100);
    while (!philo->data->someone_died)
    {
        take_forks(philo);
        eat(philo);
        sleep_and_think(philo);
    }
    return (NULL);
}
