/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/27 14:07:41 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/30 16:11:39 by pniezen       ########   odam.nl         */
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

long	get_time_running(t_info *info)
{
	return (get_time_in_ms() - info->start_time);
}

void	destroy_philo_locks(t_info *info, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&info->philos[i].philo_lock);
		i++;
	}
	free(info->philos);
	info->philos = NULL;
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
	pthread_mutex_destroy(&info->died_lock);
	pthread_mutex_destroy(&info->write_lock);
}

void	free_all(t_info *info)
{
	destroy_philo_locks(info, info->num_philos);
	destroy_locks(info, info->num_philos);
	free(info->p_threads);
	free(info->fork);
	free(info->philos);
	info->p_threads = NULL;
	info->fork = NULL;
	info->philos = NULL;
}
