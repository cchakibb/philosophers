/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:41:29 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/11 22:56:46 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    *philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->id % 2 == 0)
        usleep(philo->data->time_to_eat * 1000);
    while (!philo->data->someone_died)
    {
        pthread_mutex_lock(philo->left_fork);
        print_message(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_message(philo, "has taken a fork");
        print_message(philo, "is eating");
        philo->last_meal_time = get_time();
        usleep(philo->data->time_to_eat * 1000);
        philo->meals_eaten++;
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        print_message(philo, "is sleeping");
        usleep(philo->data->time_to_sleep * 1000);
        print_message(philo, "is thinking");
    }
    return (NULL);
}

void    *monitor_philos(void *arg)
{
    t_data  *data;
    int     i;

    data = (t_data *)arg;
    while (!data->someone_died)
    {
        i = 0;
        while (i < data->num_philos)
        {
            if (get_time() - data->philos[i].last_meal_time > data->time_to_die)
            {
                print_message(&data->philos[i], "died");
                data->someone_died = 1;
                return (NULL);
            }
            if (data->num_meals_to_eat != -1 && data->philos[i].meals_eaten >= data->num_meals_to_eat)
                data->someone_died = 1;
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}

int create_threads(t_data *data)
{
    int         i;
    pthread_t   monitor_thread;

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philo_routine, &data->philos[i]) != 0)
            return (1);
        i++;
    }
    if (pthread_create(&monitor_thread, NULL, monitor_philos, data) != 0)
        return (1);
    pthread_join(monitor_thread, NULL);
    return (0);
}