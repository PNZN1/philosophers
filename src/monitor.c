/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/30 11:06:37 by pniezen       #+#    #+#                 */
/*   Updated: 2022/12/20 14:55:04 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

static bool	enough_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_lock);
	if (!philo->info->eat_limit)
	{
		pthread_mutex_unlock(&philo->philo_lock);
		return (false);
	}
	if (philo->times_eaten >= philo->info->num_times_philo_eat)
	{
		pthread_mutex_unlock(&philo->philo_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	return (false);
}

void	monitor(t_info *info)
{
	int		i;
	long	eat_times;

	while (true)
	{
		i = 0;
		eat_times = 0;
		while (i < info->num_philos)
		{
			if (info->eat_limit && enough_eaten(&info->philos[i]))
				eat_times++;
			if (is_funeral(&info->philos[i], ME))
				return ((void)print_state(&info->philos[i], DIED));
			i++;
		}
		if (eat_times == info->num_philos)
		{
			pthread_mutex_lock(&info->died_lock);
			info->died = true;
			pthread_mutex_unlock(&info->died_lock);
			return ;
		}
	}
}

bool	is_funeral(t_philo *philo, t_state who)
{
	long	last_meal;

	pthread_mutex_lock(&philo->info->died_lock);
	pthread_mutex_lock(&philo->philo_lock);
	last_meal = philo->time_last_meal;
	pthread_mutex_unlock(&philo->philo_lock);
	if (philo->info->died && who == ME)
	{
		philo->info->died = true;
		pthread_mutex_unlock(&philo->info->died_lock);
		return (true);
	}
	if (get_time_in_ms() - last_meal >= philo->info->to_die
		&& !philo->info->died)
	{
		philo->info->died = true;
		pthread_mutex_unlock(&philo->info->died_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->info->died_lock);
	return (false);
}

void	print_state(t_philo *philo, t_state state)
{
	long	cur_time;

	pthread_mutex_lock(&philo->info->write_lock);
	cur_time = get_time_in_ms() - philo->info->start_time;
	if (state == DIED)
	{
		printf("%ld %d died\n", cur_time, philo->id);
		return ((void)pthread_mutex_unlock(&philo->info->write_lock));
	}
	if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
		return ((void)pthread_mutex_unlock(&philo->info->write_lock));
	if (state == TAKEN_FORK && !enough_eaten(philo))
		printf("%ld %d has taken a fork\n", cur_time, philo->id);
	else if (state == EATING && !enough_eaten(philo))
		printf("%ld %d is eating\n", cur_time, philo->id);
	else if (state == SLEEPING && !enough_eaten(philo))
		printf("%ld %d is sleeping\n", cur_time, philo->id);
	else if (state == THINKING && !enough_eaten(philo))
		printf("%ld %d is thinking\n", cur_time, philo->id);
	pthread_mutex_unlock(&philo->info->write_lock);
}
