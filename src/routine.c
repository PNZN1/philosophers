/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/20 14:28:27 by pniezen       #+#    #+#                 */
/*   Updated: 2022/12/20 14:41:58 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

static void	end_routine(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		pthread_join(info->p_threads[i], NULL);
		i++;
	}
	free_all(info);
}

static void	just_one(t_philo *philo)
{
	printf("0 %d has taken a fork\n", philo->id);
	usleep(philo->info->to_die);
}

static void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (is_funeral(philo, ME))
		return (NULL);
	if (philo->info->num_philos == 1)
		return (just_one(philo), NULL);
	print_state(philo, THINKING);
	if (philo->id % 2 == 0)
		usleep(200);
	while (true)
	{
		if (!eat(philo))
			break ;
		if (!sleeps(philo))
			break ;
		if (!thinking(philo))
			break ;
	}
	return (NULL);
}

void	start_routine(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		if (pthread_create(&info->p_threads[i], NULL,
				routine, &info->philos[i]))
			break ;
		i++;
	}
	monitor(info);
	end_routine(info);
}
