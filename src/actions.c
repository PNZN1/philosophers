/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/29 08:35:39 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/29 21:05:23 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

void	monitor(t_info *info)
{
	int		i;
	long	eat_times;

	while (!info->died)
	{
		i = 0;
		eat_times = 0;
		while (i < info->num_philos - 1)
		{
			if (info->num_times_philo_eat >= info->philos[i].times_eaten)
				eat_times++;
			if (is_funeral(&info->philos[i], OTHER))
				return ((void)print_state(&info->philos[i], DIED));
			i++;
		}
		if (eat_times == info->num_philos)
			return ;
	}
}

bool	is_funeral(t_philo *philo, t_state who)
{
	pthread_mutex_lock(&philo->philo_lock);
	pthread_mutex_lock(&philo->info->died_lock);
	if (philo->info->died && who == ME)
	{
		pthread_mutex_unlock(&philo->info->died_lock);
		return (true);
	}
	if (get_time_in_ms() - philo->time_last_meal >= philo->info->to_die
		&& !philo->info->died)
	{
		philo->info->died = true;
		pthread_mutex_unlock(&philo->philo_lock);
		pthread_mutex_unlock(&philo->info->died_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	pthread_mutex_unlock(&philo->info->died_lock);
	return (false);
}

static void	take_forks(t_philo *philo)
{
	if (philo->index != 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, TAKEN_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, TAKEN_FORK);
	}
}

bool	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_lock);
	if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
		return (false);
	philo->times_eaten++;
	philo->time_last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->philo_lock);
	take_forks(philo);
	print_state(philo, EATING);
	while (get_time_in_ms() - philo->time_last_meal < philo->info->to_eat)
	{
		if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
			return (false);
		usleep(200);
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (true);
}

bool	sleeps(t_philo *philo)
{
	long	cur_time;

	pthread_mutex_lock(&philo->philo_lock);
	if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
		return (false);
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
