/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/21 14:52:01 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/30 16:20:57 by pniezen       ########   odam.nl         */
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

static void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (true)
	{
		if (is_funeral(philo, ME) || is_funeral(philo, OTHER))
			break ;
		if (philo->id % 2 == 0)
			usleep(250);
		if (!eat(philo))
			break ;
		if (!sleeps(philo))
			break ;
		if (!thinking(philo))
			break ;
	}
	// usleep(200);
	return (NULL);
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
	if (info.num_philos == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(info.to_die);
		printf("%ld 1 died\n", info.to_die);
		return (0);
	}
	if (!init(&info))
		return (1);
	start_routine(&info);
	return (0);
}
