/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 22:46:06 by user42            #+#    #+#             */
/*   Updated: 2021/10/03 22:52:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	mutex_fork(int lock, t_phil *phil, int i)
{
	if (lock)
	{
		pthread_mutex_lock(&phil->info->fork[i]);
		if (i != phil->index)
			print_state("has taken a fork right\n", phil, phil->info);
		else
			print_state("has taken a fork\n", phil, phil->info);
	}
	else
	{
		pthread_mutex_unlock(&phil->info->fork[i]);
		/*if (i != phil->index)//DEBUG
			print_state("has drop a fork right\n", phil, phil->info);
		else
			print_state("has drop a fork\n", phil, phil->info);*/
		return (0);
	}
	return (0);
}

int	take_drop_fork(int take, t_phil *phil)
{
	int	i;

	i = phil->index;
	mutex_fork(take, phil, i);
	if (i != phil->info->nb_philo - 1)
		mutex_fork(take, phil, i + 1);
	else
		mutex_fork(take, phil, 0);
	return (0);
}

void	eat(t_phil *phil)
{
	if (phil->info->nb_philo == 1)
	{
		mutex_fork(1, phil, 0);
		ft_usleep(phil->info->time_die + 10000, phil->info);
		mutex_fork(0, phil, 0);
		return ;
	}
	take_drop_fork(1, phil);
	pthread_mutex_lock(&phil->check_meal);
	phil->last_eat = get_time(phil->info);
	pthread_mutex_unlock(&phil->check_meal);
	print_state("=====is eating\n", phil, phil->info);
	ft_usleep(phil->info->time_eat, phil->info);
	//if(ft_usleep(phil->info->time_eat /** 1000*/, phil->info))
	//	printf("%ld %d AWAKE from eat\n", get_time(phil->info), i);//DEBUG
	phil->count_eat++;
	if (phil->count_eat == phil->info->nb_eat)
	{
		//printf("%ld %d -----has finish\n", get_time(phil->info), i);
		pthread_mutex_lock(&phil->check_meal);
		phil->finish = 1;
		pthread_mutex_unlock(&phil->check_meal);
		usleep(20);
	}
	take_drop_fork(0, phil);
}

void	*routine(void *ptr)
{
	t_phil	*phil;
	int		i;

	phil = (t_phil *)ptr;
	i = phil->index;
	if (i % 2 != 0)
		usleep(100);
	while (!check_turn_off(phil->info) && phil->info->nb_eat != 0)
	{
		//	printf("%ld ROUTINE %d (%d)\n",get_time(phil->info), i, phil->count_eat);
		//if(!check_turn_off(phil->info))
		print_state("is thinking\n", phil, phil->info);
		if (!check_turn_off(phil->info))
			eat(phil);
		print_state("is sleeping\n", phil, phil->info);
		ft_usleep(phil->info->time_sleep, phil->info);
		//if (ft_usleep(phil->info->time_sleep /** 1000*/, phil->info))
		//	printf("%ld %dAWAKE from sleep\n", get_time(phil->info), i);//DEBUG
		//}
	}
	//printf("LEAVING routine %d (%d)\n", i, phil->count_eat);
	return (0);
}

