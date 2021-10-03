/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 22:34:26 by user42            #+#    #+#             */
/*   Updated: 2021/10/03 22:37:02 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_turn_off(t_info *info)
{
	pthread_mutex_lock(&info->check_off);
	if (info->turn_off)
	{
		pthread_mutex_unlock(&info->check_off);
		return (1);
	}
	pthread_mutex_unlock(&info->check_off);
	return (0);
}

int	ft_usleep(int time_to_sleep, t_info *info)
{
	long	start;

	start = get_time(info);
	while (get_time(info) - start < time_to_sleep && !check_turn_off(info))
	{
		usleep(100);
		if (check_turn_off(info) && get_time(info) - start < time_to_sleep )//DEBUG
			return (1);
	}
	return (0);
}

int	print_state(char *state, t_phil *phil, t_info *info)
{
	usleep(100);
	if (!check_turn_off(info))
	{	
		pthread_mutex_lock(&info->print_turn);
		printf("%ld %d %s", get_time(info), phil->index, state);
		pthread_mutex_unlock(&info->print_turn);
	}
/*	else
	{	
		pthread_mutex_lock(&info->print_turn);
		printf("(%ld %d %s)", get_time(info), phil->index, state);
		pthread_mutex_unlock(&info->print_turn);
	}*/
	return (0);
}

long	get_time(t_info *info)
{
	long	time_sec;
	long	time_msec;

	if (gettimeofday(&info->time, NULL))
		return (0);
	time_sec = info->time.tv_sec - info->start.tv_sec;
	time_msec = info->time.tv_usec / 1000 - info->start.tv_usec / 1000;
	if (time_msec < 0)
	{
		time_msec += 1000;
		time_sec -= 1;
	}
	time_sec = (time_sec * 1000) + time_msec;
	return (time_sec);
}

