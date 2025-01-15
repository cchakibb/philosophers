/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:19:10 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 23:46:47 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// philo.h
#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

# define ERROR_MESSAGE "Error\n"

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal_time;
	pthread_t			thread;
	struct s_data		*data;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
}						t_philo;

typedef struct s_data
{
	int					num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_meals_to_eat;
	long long			start_time;
	pthread_mutex_t		state_mutex;
	int					someone_died;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_mutex;
	t_philo				*philos;
}						t_data;

// init.c
int				init_philos(t_data *data);
long long		get_time(void);
int				init_mutexes(t_data *data);
int				init_and_start(t_data *data, int argc, char **argv);

// threads.c
int				create_threads(t_data *data);
void			*monitor_philos(void *arg);
void			*philo_routine(void *arg);
int				has_died(t_data *data);
void			eating_phase(t_philo *philo);
void			sleeping_thinking_phase(t_philo *philo);
int				should_continue(t_philo *philo);
void			update_last_meal(t_philo *philo);
void			increment_meals(t_philo *philo);

// utils.c
void			print_message(t_philo *philo, char *message);

#endif

