/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:56:50 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/24 20:31:50 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 1 - number_of_philosophers
// 2 - time_to_die
// 3 - time_to_eat 
// 4 - time_to_sleep
// 5 - [number_of_times_each_philosopher_must_eat]

// int main(int ac, char **av)
// {
// 	if (ac )
// }

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define INCREMENTS 10000

int counter = 0; // Variable partagée
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void* arg) {
    for(int i = 0; i < INCREMENTS; i++)
	{
		pthread_mutex_lock(&mutex);
        counter++; // Incrémentation de la variable partagée
		pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Création des threads
    for(int i = 0; i < NUM_THREADS; i++) {
        if(pthread_create(&threads[i], NULL, increment, NULL)) {
            fprintf(stderr, "Erreur lors de la création du thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Attente de la fin des threads
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
	pthread_mutex_destroy(&mutex);
    // Affichage du résultat
    printf("Valeur finale de counter : %d\n", counter);
    return 0;
}
