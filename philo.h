/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:19:10 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/01 22:05:06 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define ERROR_MESSAGE "Error\n"

typedef struct s_philo
{
    int             id;
    int             meals_eaten;
    long long       last_meal_time;
    pthread_t       thread;
    struct s_data   *data;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
}               t_philo;

typedef struct s_data
{
    int             num_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             num_meals_to_eat;
    long long       start_time;
    int             someone_died;
    pthread_mutex_t *forks;
    pthread_mutex_t write_mutex;
    t_philo         *philos;
}               t_data;

// init.c
int init_philos(t_data *data);
long long get_time(void);
int init_mutexes(t_data *data);
int init_data(t_data *data, int argc, char **argv);
int ft_atoi(const char *str);

//threads.c
int create_threads(t_data *data);
void    *monitor_philos(void *arg);
void    *philo_routine(void *arg);

// utils.c
void	print_message(t_philo *philo, char *message);

#endif