/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaamir <malaamir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 23:35:31 by malaamir          #+#    #+#             */
/*   Updated: 2025/04/10 10:20:07 by malaamir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_think(t_philo *philo)
{
	send_msg("is thinking", philo, philo->id);
}

void	ft_sleep(t_philo *philo)
{
	send_msg("is sleeping", philo, philo->id);
	ft_pause(philo, philo->time_to_sleep);
}

void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	send_msg("has taken a fork", philo, philo->id);
	if (philo->philo_nums == 1)
	{
		ft_pause(philo, philo->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	if (philo->num_of_eat != -1 && philo->meals_eaten >= philo->num_of_eat)
		return ;
	pthread_mutex_lock(philo->left_fork);
	send_msg("has taken a fork", philo, philo->id);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_time();
	philo->eating = 1;
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	send_msg("is eating", philo, philo->id);
	ft_pause(philo, philo->time_to_eat);
	pthread_mutex_lock(philo->meal_lock);
	philo->eating = 0;
	pthread_mutex_unlock(philo->meal_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		usleep(400);
	while (!check_dead_loop(philo))
	{
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	return (ptr);
}
