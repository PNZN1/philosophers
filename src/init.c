/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/27 13:54:09 by pniezen       #+#    #+#                 */
/*   Updated: 2022/12/20 12:32:12 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>

static bool	init_forks(t_info *info)
{
	int	i;

	info->fork = malloc(sizeof(pthread_mutex_t) * info->num_philos);
	if (!info->fork)
		return (false);
	i = 0;
	while (i < info->num_philos)
	{
		if (pthread_mutex_init(&info->fork[i], NULL) != 0)
			return (destroy_locks(info, i), false);
		i++;
	}
	return (true);
}

static bool	init_philos(t_info *info)
{
	int	i;

	info->philos = malloc(sizeof(t_philo) * info->num_philos);
	if (!info->num_philos)
		return (false);
	i = 0;
	while (i < info->num_philos)
	{
		info->philos[i].info = info;
		info->philos[i].id = i + 1;
		info->philos[i].time_last_meal = get_time_in_ms();
		info->philos[i].times_eaten = 0;
		info->philos[i].right_fork = &info->fork[i];
		info->philos[i].left_fork = &info->fork[(i + 1) % info->num_philos];
		if (pthread_mutex_init(&info->philos[i].philo_lock, NULL) != 0)
			return (destroy_philo_locks(info, i), false);
		i++;
	}
	return (true);
}

bool	init(t_info *info)
{
	if (pthread_mutex_init(&info->died_lock, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&info->write_lock, NULL) != 0)
		return (pthread_mutex_destroy(&info->died_lock), false);
	info->start_time = get_time_in_ms();
	info->died = false;
	info->p_threads = malloc(sizeof(pthread_t) * info->num_philos);
	if (!info->p_threads)
		return (false);
	if (!init_forks(info))
		return (false);
	if (!init_philos(info))
		return (free_all(info), false);
	return (true);
}
