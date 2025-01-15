/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:26:45 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:52:50 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_unlock(philo->left_fork);
	while (!has_died(philo->data))
	{
		if (get_time() - philo->last_meal_time > philo->data->time_to_die)
			break ;
		usleep(100);
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->data->num_philos == 1)
		return (handle_single_philo(philo), NULL);
	while (should_continue(philo))
	{
		if (!should_continue(philo))
			break ;
		eating_phase(philo);
		if (!should_continue(philo))
			break ;
		sleeping_thinking_phase(philo);
	}
	return (NULL);
}

int	should_continue(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	if (philo->data->someone_died || \
		(philo->data->num_meals_to_eat != -1 && \
		philo->meals_eaten >= philo->data->num_meals_to_eat))
	{
		pthread_mutex_unlock(&philo->data->state_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->state_mutex);
	return (1);
}

void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->state_mutex);
}
