/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/27 13:08:37 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/30 13:44:45 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdio.h>

static bool	check_is_digit(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("'%s' is not a digit.\n\n", argv[i]);
				printf(WRONG_ARGUMENTS);
				return (false);
			}
			j++;
		}
		i++;
	}
	return (true);
}

static long	arg_to_long(char *arg)
{
	int		i;
	long	res;
	long	max;

	if (!arg)
		return (-1);
	i = 0;
	res = 0;
	max = INT_MAX;
	while (arg[i] && arg[i] >= '0' && arg[i] <= '9')
	{
		res = res * 10 + (arg[i] - '0');
		if (res > max)
			return (-1);
		i++;
	}
	if (arg[i])
		return (-1);
	return (res);
}

static void	set_thinking_time(t_info *info)
{
	if (info->num_philos % 2 == 1 && (info->to_eat * 2) > info->to_sleep)
		info->to_think = (info->to_eat * 2) - info->to_sleep;
	else
		info->to_think = 0;
}

bool	parse_arguments(int argc, char **argv, t_info *info)
{
	if (!info)
		return (false);
	if (!check_is_digit(argc, argv))
		return (false);
	info->num_philos = arg_to_long(argv[1]);
	info->to_die = arg_to_long(argv[2]);
	info->to_eat = arg_to_long(argv[3]);
	info->to_sleep = arg_to_long(argv[4]);
	set_thinking_time(info);
	info->num_times_philo_eat = -1;
	info->eat_limit = false;
	if (argc == 6)
		info->num_times_philo_eat = arg_to_long(argv[5]);
	if (info->num_philos < 0 || info->to_die < 0 || info->to_eat < 0
		|| info->to_sleep < 0
		|| (argc == 6 && info->num_times_philo_eat < 0))
		return (false);
	if (info->num_times_philo_eat != -1)
		info->eat_limit = true;
	return (true);
}
