/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:41:29 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:07:34 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// threads.c
#include "philo.h"

static void	set_died(t_data *data)
{
	pthread_mutex_lock(&data->state_mutex);
	data->someone_died = 1;
	pthread_mutex_unlock(&data->state_mutex);
}

int	has_died(t_data *data)
{
	int	died;

	pthread_mutex_lock(&data->state_mutex);
	died = data->someone_died;
	pthread_mutex_unlock(&data->state_mutex);
	return (died);
}

static void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->state_mutex);
}

static void	increment_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->state_mutex);
}

static int	should_continue(t_philo *philo)
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

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->data->num_philos == 1)
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
		return (NULL);
	}
	while (should_continue(philo))
	{
		if (!should_continue(philo))
			break ;
		take_forks(philo);

		print_message(philo, "is eating");
		update_last_meal(philo);
		usleep(philo->data->time_to_eat * 1000);
		increment_meals(philo);
		release_forks(philo);
		if (!should_continue(philo))
			break ;
		print_message(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		if (!should_continue(philo))
			break ;
		print_message(philo, "is thinking");
		usleep(500);
	}
	return (NULL);
}

static int	check_death(t_philo *philo)
{
	long long	now;
	long long	last_meal_time;
	int	time_to_die;

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

void	*monitor_philos(void *arg)
{
	t_data	*data;
	int	i;

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

