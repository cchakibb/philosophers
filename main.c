/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:20:51 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:52:20 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_resources(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	free(data->forks);
	free(data->philos);
}

static int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
		{
			write(2, ERROR_MESSAGE, 6);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		write(2, ERROR_MESSAGE, 6);
		return (1);
	}
	if (init_and_start(&data, argc, argv) != 0)
	{
		write(2, ERROR_MESSAGE, 6);
		return (1);
	}
	while (!has_died(&data))
		usleep(1000);
	if (join_threads(&data) != 0)
		return (1);
	cleanup_resources(&data);
	return (0);
}
