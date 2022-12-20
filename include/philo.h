/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pniezen <pniezen@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/22 09:57:35 by pniezen       #+#    #+#                 */
/*   Updated: 2022/12/20 14:58:39 by pniezen       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <pthread.h>

# define WRONG_ARGUMENTS "You didn't give the right arguments\ne.g.:\
\n./philo 5 100 80 100\n"
# define MAX_AMOUNT_PHILO "Maximum amount of philosophers is 200.\n\n"

typedef struct s_philo	t_philo;
typedef struct s_info	t_info;

struct s_philo
{
	t_info			*info;
	pthread_mutex_t	philo_lock;
	int				id;
	long			time_last_meal;
	long			times_eaten;
	pthread_mutex_t	time_eat_lock;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
};

struct s_info
{
	t_philo			*philos;
	pthread_t		*p_threads;
	long			num_philos;
	long			to_die;
	long			to_eat;
	long			to_sleep;
	long			to_think;
	long			num_times_philo_eat;
	bool			eat_limit;
	long			start_time;
	bool			died;
	pthread_mutex_t	died_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork;
};

typedef enum e_state
{
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	ME,
	OTHER
}	t_state;

//parsing.c

bool	parse_arguments(int argc, char **argv, t_info *info);

//init.c

bool	init(t_info *info);

//utils.c

long	get_time_in_ms(void);
long	get_time_running(t_info *info);
void	destroy_philo_locks(t_info *info, int n);
void	destroy_locks(t_info *info, int n);
void	free_all(t_info *info);

//routine.c

void	start_routine(t_info *info);

//actions.c

bool	eat(t_philo *philo);
bool	sleeps(t_philo *philo);
bool	thinking(t_philo *philo);

//monitor.c

void	monitor(t_info *info);
bool	is_funeral(t_philo *philo, t_state who);
void	print_state(t_philo *philo, t_state state);

#endif
