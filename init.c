/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 22:38:59 by user42            #+#    #+#             */
/*   Updated: 2021/10/03 22:49:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_info *info, t_phil **phil)
{
	int i;

	i = 0;
	while (i < info->nb_philo)
	{
		(*phil)[i].info = info;
		(*phil)[i].count_eat = 0;
		(*phil)[i].index = i;
		(*phil)[i].last_eat = 0;
		(*phil)[i].finish = 0;
		i++;
	}
}

int	init(t_info *info, t_phil **phil)
{
	int	i;

	i = 0;
	*phil = malloc(sizeof(t_phil) * info->nb_philo);
	info->fork = malloc(sizeof(pthread_mutex_t) * info->nb_philo);
	if (!*phil || !info->fork)
		return (1);
	info->turn_off = 0;
	while (i < info->nb_philo)
	{
		if (pthread_mutex_init(&info->fork[i], NULL))
			return (1);
		if (pthread_mutex_init(&(*phil)[i].check_meal, NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&info->print_turn, NULL))
		return (1);
	if (pthread_mutex_init(&info->check_off, NULL))
		return (1);
	init_philo(info, phil);
	return (0);
}

int	check_data(char *argv, int index)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (!ft_isdigit(argv[i]))
		{	
			printf("enter a positive number please\n");
			return (0);
		}
		if (i > 10)
		{
			printf("enter an integer please\n");
			return (0);
		}
		if (index == 1 && i > 2)
		{
			printf("be gentle : no more than 200 philosopher can have dinner at the same time\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse(t_info *info, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!check_data(argv[i], i))
		{
			//printf("parsing error\n");
			return (1);
		}
		i++;
	}
	info->nb_philo = ft_atoi(argv[1]);
	info->time_die = ft_atoi(argv[2]);
	info->time_eat = ft_atoi(argv[3]);
	info->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
		info->nb_eat = ft_atoi(argv[5]);
	else
		info->nb_eat = -1;
	return (0);
}

int	exit_philo(t_phil **phil, t_info *info, int exit_val)
{
	int	i;

	i = 0;
	while (i < info->nb_philo)
	{
		//printf("JOIN %d\n", i);
		pthread_join((*phil)[i].philo, NULL);
		i++;
	}
	i = 0;
	while (i < info->nb_philo)
	{
	//	printf("MUTEX DESTROY %d\n", i);
		pthread_mutex_destroy(&info->fork[i]);
		pthread_mutex_destroy(&(*phil)[i].check_meal);
		i++;
	}
	pthread_mutex_destroy(&info->check_off);
	ft_free((void **)&info->fork);
	ft_free((void **)phil);
	return (exit_val);
}
