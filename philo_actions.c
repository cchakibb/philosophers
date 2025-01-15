/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:25:23 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:52:42 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if ((size_t)philo->left_fork < (size_t)philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork");
	}
}

static void	release_forks(t_philo *philo)
{
	if ((size_t)philo->left_fork < (size_t)philo->right_fork)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	eating_phase(t_philo *philo)
{
	take_forks(philo);
	print_message(philo, "is eating");
	update_last_meal(philo);
	usleep(philo->data->time_to_eat * 1000);
	increment_meals(philo);
	release_forks(philo);
}

void	sleeping_thinking_phase(t_philo *philo)
{
	print_message(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
	if (!should_continue(philo))
		return ;
	print_message(philo, "is thinking");
	usleep(500);
}
