/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:41:29 by chbachir          #+#    #+#             */
/*   Updated: 2025/01/15 11:58:11 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// threads.c
#include "philo.h"

/* ------------------ Fonctions auxiliaires protégées ------------------ */

static void set_died(t_data *data)
{
    pthread_mutex_lock(&data->state_mutex);
    data->someone_died = 1;
    pthread_mutex_unlock(&data->state_mutex);
}

static void update_last_meal(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->state_mutex);
    philo->last_meal_time = get_time();
    pthread_mutex_unlock(&philo->data->state_mutex);
}

static void increment_meals(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->state_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->state_mutex);
}

static long long get_last_meal(t_philo *philo)
{
    long long tmp;

    pthread_mutex_lock(&philo->data->state_mutex);
    tmp = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->state_mutex);
    return (tmp);
}

static int should_continue(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->state_mutex);
    if (philo->data->someone_died || 
        (philo->data->num_meals_to_eat != -1 && 
         philo->meals_eaten >= philo->data->num_meals_to_eat))
    {
        pthread_mutex_unlock(&philo->data->state_mutex);
        return (0);
    }
    pthread_mutex_unlock(&philo->data->state_mutex);
    return (1);
}

/*
 * Ajout de checks supplémentaires après la prise de chaque fourchette
 * et juste avant d'incrémenter les repas. Ainsi, si la simulation
 * s'arrête au moment critique, le philosophe libère immédiatement
 * ses fourchettes et ne compte pas de repas supplémentaire.
 */
void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    // Cas spécial pour un seul philosophe
    if (philo->data->num_philos == 1)
    {
        pthread_mutex_lock(philo->left_fork);
        print_message(philo, "has taken a fork");
        while (!has_died(philo->data))
            usleep(100);
        pthread_mutex_unlock(philo->left_fork);
        return (NULL);
    }

    // Décalage pour les philosophes pairs
    if (philo->id % 2 == 0)
        usleep(200);

    // Déterminer l'ordre des fourchettes (la plus petite en premier)
    if (philo->left_fork < philo->right_fork)
    {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    while (!has_died(philo->data) && should_continue(philo))
    {
        /* ---- Prise de la première fourchette ---- */
        pthread_mutex_lock(first_fork);
        if (has_died(philo->data))
        {
            pthread_mutex_unlock(first_fork);
            break;
        }
        print_message(philo, "has taken a fork");

        /* ---- Prise de la deuxième fourchette ---- */
        pthread_mutex_lock(second_fork);
        if (has_died(philo->data))
        {
            pthread_mutex_unlock(second_fork);
            pthread_mutex_unlock(first_fork);
            break;
        }
        print_message(philo, "has taken a fork");

        /* ---- Manger ---- */
        if (!has_died(philo->data))
        {
            print_message(philo, "is eating");
            update_last_meal(philo);
            usleep(philo->data->time_to_eat * 1000);

            if (!has_died(philo->data))
                increment_meals(philo);
        }

        // Libération des fourchettes
        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);

        if (has_died(philo->data))
            break;

        /* ---- Dormir ---- */
        print_message(philo, "is sleeping");
        usleep(philo->data->time_to_sleep * 1000);

        /* ---- Penser ---- */
        print_message(philo, "is thinking");
    }
    return (NULL);
}

/*
 * Vérifie si tous les philosophes ont mangé suffisamment
 */
static int all_philos_ate_enough(t_data *data)
{
    int i;
    
	i = 0;
    if (data->num_meals_to_eat == -1)
        return (0);
        
    pthread_mutex_lock(&data->state_mutex);
    while (i < data->num_philos)
    {
        if (data->philos[i].meals_eaten < data->num_meals_to_eat)
        {
            pthread_mutex_unlock(&data->state_mutex);
            return (0);
        }
		i++;
    }
    pthread_mutex_unlock(&data->state_mutex);
    return (1);
}

/*
 * Monitor : on réduit la période de sommeil à 200 µs
 * pour réagir plus rapidement et limiter le cas
 * où un philo entame un 8e repas juste avant d’être stoppé.
 */
void *monitor_philos(void *arg)
{
    t_data  *data = (t_data *)arg;
    int     i;

    while (!has_died(data))
    {
        i = 0;
        if (all_philos_ate_enough(data))
        {
            set_died(data);
            break;
        }
        
        while (i < data->num_philos)
        {
            if ((get_time() - get_last_meal(&data->philos[i])) > data->time_to_die)
            {
                print_message(&data->philos[i], "died");
                set_died(data);
                break;
            }
            i++;
        }
        usleep(100); // Réduire à 100 microsecondes pour plus de réactivité
    }
    return (NULL);
}

int create_threads(t_data *data)
{
    int         i;
    pthread_t   monitor_thread;

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philo_routine, &data->philos[i]) != 0)
            return (1);
        i++;
    }
    // Thread moniteur
    if (pthread_create(&monitor_thread, NULL, monitor_philos, data) != 0)
        return (1);

    // Attend la fin du moniteur (qui return après set_died)
    pthread_join(monitor_thread, NULL);
    return (0);
}
