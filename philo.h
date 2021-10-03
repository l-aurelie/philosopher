/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 20:27:42 by user42            #+#    #+#             */
/*   Updated: 2021/10/03 23:45:56 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_info
{
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print_turn;
	pthread_mutex_t	check_off;
	int				turn_off;
	struct timeval	time;
	struct timeval	start;
}	t_info;

typedef struct s_phil
{
	int				index;
	pthread_t		philo;
	int				count_eat;
	long			last_eat;
	int				finish;
	pthread_mutex_t	check_meal;
	t_info			*info;
}	t_phil;

/*
** LIB_UTILS
*/
int		ft_strlen(char *s);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
int		ft_free(void **ptr);

/*
** PHILO_UTILS
*/
int		check_turn_off(t_info *info);
int		ft_usleep(int time_to_sleep, t_info *info);
int		print_state(char *state, t_phil *phil, t_info *info);
long	get_time(t_info *info);

/*
** INIT_EXIT
*/
void	init_philo(t_info *info, t_phil **phil);
int		init(t_info *info, t_phil **phil);
int		check_data(char *argv, int index);
int		parse(t_info *info, char **argv);
int		exit_philo(t_phil **phil, t_info *info, int exit_val);

/*
** SIMULATION
*/
int		mutex_fork(int lock, t_phil *phil, int i);
int		take_drop_fork(int take, t_phil *phil);
void	eat(t_phil *phil);
void	*routine(void *ptr);

/*
** LIB_UTILS
*/
int		checker(t_info *info, t_phil **phil);
void	philo(t_info *info, t_phil **phil);

#endif
