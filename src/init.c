/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/27 13:54:09 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/29 21:02:34 by pniezen       ########   odam.nl         */
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
			return (false);
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
		info->philos[i].index = i + 1;
		info->philos[i].time_last_meal = get_time_in_ms();
		info->philos[i].times_eaten = 0;
		info->philos[i].left_fork = &info->fork[i];
		info->philos[i].right_fork = &info->fork[(i + 1) % info->num_philos];
		i++;
	}
	return (true);
}

bool	init(t_info *info)
{
	info->done = false;
	info->start_time = get_time_in_ms();
	if (info->start_time < 200)
		return (false);
	info->p_threads = malloc(sizeof(pthread_t) * info->num_philos);
	if (!info->p_threads)
		return (false);
	if (!init_forks(info))
		return (false);
	if (!init_philos(info))
		return (free_all(info), false);
	return (true);
}
