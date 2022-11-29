/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/21 14:52:01 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/29 20:48:04 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

static void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (true)
	{
		if (philo->index % 2 == 0)
			usleep(200);
		if (!eat(philo))
			break ;
		if (!sleeps(philo))
			break ;
		print_state(philo, THINKING);
	}
	return (false);
}

static void	start_routine(t_info *info)
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

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc < 5 || argc > 6)
		return ((void)printf(WRONG_ARGUMENTS), 1);
	if (!parse_arguments(argc, argv, &info))
		return (1);
	if (!init(&info))
		return (1);
	start_routine(&info);
	return (0);
}
