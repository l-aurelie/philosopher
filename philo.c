/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 20:13:12 by user42            #+#    #+#             */
/*   Updated: 2021/10/04 00:52:31 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	checker(t_info *info, t_phil **phil)
{
	int	i;
	int	finish;

	i = 0;
	finish = 1;
	while (i < info->nb_philo)
	{
		pthread_mutex_lock(&(*phil)[i].check_meal);
		if ((*phil)[i].finish == 0)
			finish = 0;
		pthread_mutex_unlock(&(*phil)[i].check_meal);
		pthread_mutex_lock(&(*phil)[i].check_meal);
		if (get_time(info) - (*phil)[i].last_eat >= info->time_die)
		{
			print_state("died\n", *phil, info);
			pthread_mutex_unlock(&(*phil)[i].check_meal);
			return (1);
		}
		pthread_mutex_unlock(&(*phil)[i].check_meal);
		i++;
	}
	if (finish)
		return (1);
	return (0);
}

void	philo(t_info *info, t_phil **phil)
{
	int	i;

	i = 0;
	gettimeofday(&info->start, NULL);
	while (i < info->nb_philo)
	{
		if (!i % 2 == 0)
			usleep(100);
		pthread_create(&(*phil)[i].philo, NULL, &routine, (void *)&(*phil)[i]);
		i++;
	}
	while (1)
	{
		usleep(500);
		if (checker(info, phil))
		{
			pthread_mutex_lock(&info->check_off);
			info->turn_off = 1;
			pthread_mutex_unlock(&info->check_off);
			break ;
		}
	}
}

int	main(int argc, char **argv)
{
	t_info	info;
	t_phil	*phil;

	if (argc == 6 || argc == 5)
	{
		if (parse(&info, argv))
			return (1);
		if (!init(&info, &phil))
		{
			philo(&info, &phil);
			return (exit_philo(&phil, &info, 0));
		}	
	}
	else
	{
		printf("wrong number of arguments\n");
		return (1);
	}
	return (0);
}
