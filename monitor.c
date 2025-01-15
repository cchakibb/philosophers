/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:29:12 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:52:33 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_philo *philo)
{
	long long	now;
	long long	last_meal_time;
	int			time_to_die;

	pthread_mutex_lock(&philo->data->state_mutex);
	if (philo->data->someone_died)
		return (pthread_mutex_unlock(&philo->data->state_mutex), 1);
	now = get_time();
	last_meal_time = philo->last_meal_time;
	time_to_die = philo->data->time_to_die;
	pthread_mutex_unlock(&philo->data->state_mutex);
	if ((now - last_meal_time) > time_to_die)
	{
		pthread_mutex_lock(&philo->data->write_mutex);
		pthread_mutex_lock(&philo->data->state_mutex);
		if (!philo->data->someone_died)
		{
			printf("%lld %d died\n", now - philo->data->start_time, philo->id);
			philo->data->someone_died = 1;
		}
		pthread_mutex_unlock(&philo->data->state_mutex);
		pthread_mutex_unlock(&philo->data->write_mutex);
		return (1);
	}
	return (0);
}

static int	all_ate_enough(t_data *data)
{
	int	i;
	int	all_ate;

	if (data->num_meals_to_eat == -1)
		return (0);

	pthread_mutex_lock(&data->state_mutex);
	all_ate = 1;
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->num_meals_to_eat)
		{
			all_ate = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->state_mutex);
	return (all_ate);
}

static void	set_died(t_data *data)
{
	pthread_mutex_lock(&data->state_mutex);
	data->someone_died = 1;
	pthread_mutex_unlock(&data->state_mutex);
}

void	*monitor_philos(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!has_died(data))
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_death(&data->philos[i]))
				return (NULL);
			i++;
		}
		if (all_ate_enough(data))
		{
			set_died(data);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	has_died(t_data *data)
{
	int	died;

	pthread_mutex_lock(&data->state_mutex);
	died = data->someone_died;
	pthread_mutex_unlock(&data->state_mutex);
	return (died);
}
