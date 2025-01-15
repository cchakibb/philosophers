/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:41:04 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 10:11:04 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// utils.c
#include "philo.h"

/*
 * Vérification protégée de someone_died dans print_message,
 * pour éviter d’afficher quand la simulation est finie.
 */
int has_died(t_data *data)
{
    int val;

    pthread_mutex_lock(&data->state_mutex);
    val = data->someone_died;
    pthread_mutex_unlock(&data->state_mutex);
    return (val);
}

void print_message(t_philo *philo, char *message)
{
    long long time;

    pthread_mutex_lock(&philo->data->write_mutex);
    if (!has_died(philo->data))
    {
        time = get_time() - philo->data->start_time;
        printf("%lld %d %s\n", time, philo->id, message);
    }
    pthread_mutex_unlock(&philo->data->write_mutex);
}
