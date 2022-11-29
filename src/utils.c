/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/27 14:07:41 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/29 20:21:03 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

long	get_time_in_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_state(t_philo *philo, t_state state)
{
	long	cur_time;

	pthread_mutex_lock(&philo->info->write_lock);
	cur_time = get_time_in_ms() - philo->info->start_time;
	if (state == DIED)
	{
		usleep(300);
		printf("%ld %d died\n", cur_time, philo->index);
	}
	if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
		return ;
	if (state == TAKEN_FORK)
		printf("%ld %d has taken a fork\n", cur_time, philo->index);
	else if (state == EATING)
		printf("%ld %d is eating\n", cur_time, philo->index);
	else if (state == SLEEPING)
		printf("%ld %d is sleeping\n", cur_time, philo->index);
	else if (state == THINKING)
		printf("%ld %d is thinking\n", cur_time, philo->index);
	else if (state == DIED)
		printf("%ld %d died\n", cur_time, philo->index);
	pthread_mutex_unlock(&philo->info->write_lock);
}

void	destroy_locks(t_info *info, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&info->fork[i]);
		i++;
	}
	free(info->fork);
	info->fork = NULL;
}

void	free_all(t_info *info)
{
	destroy_locks(info, info->num_philos);
	free(info->p_threads);
	free(info->fork);
	free(info->philos);
	info->p_threads = NULL;
	info->fork = NULL;
	info->philos = NULL;
}
