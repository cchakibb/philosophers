/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:41:12 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:51:54 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(const char *str)
{
	int		i;
	long	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)res);
}

static int	init_data(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->num_meals_to_eat = ft_atoi(argv[5]);
	else
		data->num_meals_to_eat = -1;
	data->someone_died = 0;
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (1);
	return (0);
}

int	init_and_start(t_data *data, int argc, char **argv)
{
	if (init_data(data, argc, argv) != 0)
		return (1);
	if (init_mutexes(data) != 0)
		return (1);
	if (init_philos(data) != 0)
		return (1);
	if (create_threads(data) != 0)
		return (1);
	return (0);
}
