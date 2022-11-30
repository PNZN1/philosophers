/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/29 08:35:39 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/30 15:58:12 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

static void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_state(philo, TAKEN_FORK);
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, TAKEN_FORK);
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

bool	eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->philo_lock);
	philo->time_last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->philo_lock);
	print_state(philo, EATING);
	while (get_time_in_ms() - philo->time_last_meal < philo->info->to_eat)
	{
		if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
			return (drop_forks(philo), false);
		usleep(200);
	}
	philo->times_eaten++;
	return (drop_forks(philo), true);
}

bool	sleeps(t_philo *philo)
{
	long	cur_time;

	pthread_mutex_lock(&philo->philo_lock);
	pthread_mutex_unlock(&philo->philo_lock);
	print_state(philo, SLEEPING);
	cur_time = get_time_in_ms();
	while (get_time_in_ms() - cur_time < philo->info->to_sleep)
	{
		if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
			return (false);
		usleep(200);
	}
	return (true);
}

bool	thinking(t_philo *philo)
{
	long	cur_time;

	pthread_mutex_lock(&philo->philo_lock);
	pthread_mutex_unlock(&philo->philo_lock);
	print_state(philo, THINKING);
	cur_time = get_time_in_ms();
	while (get_time_in_ms() - cur_time < philo->info->to_think)
	{
		if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
			return (false);
		usleep(200);
	}
	return (true);
}
