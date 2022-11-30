/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/30 11:06:37 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/30 16:58:43 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

static bool	enough_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_lock);
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

	while (!info->died)
	{
		i = 0;
		eat_times = 0;
		while (i < info->num_philos - 1)
		{
			if (info->eat_limit && enough_eaten(&info->philos[i]))
				eat_times++;
			if (is_funeral(&info->philos[i], ME))
				return ((void)print_state(&info->philos[i], DIED));
			i++;
		}
		if (eat_times == info->num_philos -1)
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
	pthread_mutex_lock(&philo->info->died_lock);
	if (philo->info->died && who == ME)
	{
		philo->info->died = true;
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
		usleep(200);
		printf("%ld %d died\n", cur_time, philo->id);
		pthread_mutex_unlock(&philo->info->write_lock);
		return ;
	}
	if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
		return ((void)pthread_mutex_unlock(&philo->info->write_lock));
	if (state == TAKEN_FORK)
		printf("%ld %d has taken a fork\n", cur_time, philo->id);
	else if (state == EATING)
		printf("%ld %d is eating\n", cur_time, philo->id);
	else if (state == SLEEPING)
		printf("%ld %d is sleeping\n", cur_time, philo->id);
	else if (state == THINKING)
		printf("%ld %d is thinking\n", cur_time, philo->id);
	pthread_mutex_unlock(&philo->info->write_lock);
}
