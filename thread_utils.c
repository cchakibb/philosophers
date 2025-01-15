/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:30:49 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:46:37 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_data *data)
{
	pthread_t	monitor_thread;
	int			i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, \
			NULL, philo_routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_philos, data) != 0)
		return (1);
	pthread_detach(monitor_thread);
	return (0);
}

void	increment_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->state_mutex);
}

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
