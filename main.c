/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:20:51 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/31 18:31:26 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 5 && argc != 6)
    {
        write(2, ERROR_MESSAGE, 6);
        return (1);
    }
    if (init_data(&data, argc, argv) != 0)
    {
        write(2, ERROR_MESSAGE, 6);
        return (1);
    }
    if (init_mutexes(&data) != 0)
    {
        write(2, ERROR_MESSAGE, 6);
        return (1);
    }
    if (init_philos(&data) != 0)
    {
        write(2, ERROR_MESSAGE, 6);
        return (1);
    }
    if (create_threads(&data) != 0)
    {
        write(2, ERROR_MESSAGE, 6);
        return (1);
    }
    int i = 0;
    while (i < data.num_philos)
    {
        pthread_join(data.philos[i].thread, NULL);
        i++;
    }
	i = 0;
    while (i < data.num_philos)
    {
        pthread_mutex_destroy(&data.forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data.write_mutex);
    free(data.forks);
    free(data.philos);
    return (0);
}