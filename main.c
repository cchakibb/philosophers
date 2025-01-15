/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:20:51 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/14 19:41:40 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
    t_data data;
    int    i;

    if ((argc != 5 && argc != 6) || init_data(&data, argc, argv) != 0)
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

    // Attendre que tous les philosophes terminent
    i = 0;
    while (i < data.num_philos)
    {
        if (pthread_detach(data.philos[i].thread) != 0)
        {
            write(2, ERROR_MESSAGE, 6);
            return (1);
        }
        i++;
    }

    // Attendre que le moniteur signale la fin
    while (!has_died(&data))
        usleep(1000);

    // Nettoyage des ressources
    i = 0;
    while (i < data.num_philos)
        pthread_mutex_destroy(&data.forks[i++]);
    pthread_mutex_destroy(&data.write_mutex);
    pthread_mutex_destroy(&data.state_mutex);

    free(data.forks);
    free(data.philos);
    return (0);
}