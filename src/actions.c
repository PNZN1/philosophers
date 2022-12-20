/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/29 08:35:39 by pniezen       #+#    #+#                 */
/*   Updated: 2022/12/20 14:20:21 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

static void	take_forks(t_philo *philo)
{
	if (philo->id != 1)
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, TAKEN_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, TAKEN_FORK);
	}
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

bool	eat(t_philo *philo)
{
	long	last_meal;

	take_forks(philo);
	pthread_mutex_lock(&philo->philo_lock);
	philo->time_last_meal = get_time_in_ms();
	last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->philo_lock);
	print_state(philo, EATING);
	while (get_time_in_ms() - last_meal < philo->info->to_eat)
	{
		if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
			return (drop_forks(philo), false);
		usleep(200);
	}
	pthread_mutex_lock(&philo->philo_lock);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->philo_lock);
	return (drop_forks(philo), true);
}

bool	sleeps(t_philo *philo)
{
	long	cur_time;

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
