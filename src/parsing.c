/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/27 13:08:37 by pniezen       #+#    #+#                 */
/*   Updated: 2022/11/29 18:49:39 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

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

bool	parse_arguments(int argc, char **argv, t_info *info)
{
	if (!info)
		return (false);
	info->num_philos = arg_to_long(argv[1]);
	info->to_die = arg_to_long(argv[2]);
	info->to_eat = arg_to_long(argv[3]);
	info->to_sleep = arg_to_long(argv[4]);
	info->num_times_philo_eat = -1;
	if (argc == 6)
		info->num_times_philo_eat = arg_to_long(argv[5]);
	if (info->num_philos < 0 || info->to_die < 0 || info->to_eat < 0
		|| info->to_sleep < 0
		|| (argc == 6 && info->num_times_philo_eat < 0))
		return (false);
	return (true);
}
