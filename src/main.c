/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/21 14:52:01 by pniezen       #+#    #+#                 */
/*   Updated: 2022/12/20 14:39:45 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc < 5 || argc > 6)
		return ((void)printf(WRONG_ARGUMENTS), 1);
	if (!parse_arguments(argc, argv, &info))
		return ((void)printf(WRONG_ARGUMENTS), 1);
	if (!init(&info))
		return (1);
	start_routine(&info);
	return (0);
}
