/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:41:04 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 22:42:44 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// utils.c
#include "philo.h"

void	print_message(t_philo *philo, char *message)
{
	long long	time;

	pthread_mutex_lock(&philo->data->write_mutex);
	pthread_mutex_lock(&philo->data->state_mutex);
	if (!philo->data->someone_died)
	{
		time = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", time, philo->id, message);
	}
	pthread_mutex_unlock(&philo->data->state_mutex);
	pthread_mutex_unlock(&philo->data->write_mutex);
}
