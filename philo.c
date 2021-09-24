int	ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		nb = nb * 10 + (str[i] - 48);
		i++;
	}
	return (nb * sign);
}

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_info
{
	int nb_philo;
	int time_die;
	int time_eat;
	int time_sleep;
	int nb_eat;
	pthread_mutex_t *fork;
	int tue_la_mif_frere;
	struct timeval time;
	struct timeval start;
}				t_info;

typedef struct	s_phil
{
	int index;
	pthread_t philo;
	int count_eat;
	long last_eat;
	int finish;
	t_info *info;
}				t_phil;

int		destroy(t_info *info, t_phil **phil);

int		exit_philo(t_info *info, t_phil **phil, int failure)
{
	printf("EXIT TA MERE\n");
	//free;
	info->tue_la_mif_frere = 1;
	destroy(info, phil);
	if (failure)
		exit(1);
	exit(0);
	return (0);
}

long 	get_time(t_info *info)
{
	//printf("GETTIME\n");
	int i;
	long time_sec;
	long time_msec;
	long tmp;

	i = 0;
	if (gettimeofday(&info->time, NULL))
		return (0);
	time_sec = info->time.tv_sec - info->start.tv_sec;
	printf("sec => %lu - %lu = %lu\n", info->time.tv_sec, info->start.tv_sec, time_sec);
	//printf("start = %lu, time sec = %lu, res = %lu\n",info->start, info->time.tv_sec, time_res);
	time_msec = info->time.tv_usec / 1000 - info->start.tv_usec / 1000;
	printf("msec => %lu - %lu = %lu\n", info->time.tv_usec / 1000, info->start.tv_usec /1000, time_msec);
	if (time_msec < 0)
	{
		time_msec += 1000;
		time_sec -= 1;
	}
	tmp = time_msec;
	printf("tmp = %ld\n", tmp);
	while (tmp > 0)
	{
		tmp /= 10;
		i++;		
	}
	time_sec = (time_sec * (i * 10)) + time_msec;
	printf("GET TIME = %ld\n", time_sec);
	return (time_sec);
}

int		mutex_fork(int lock, t_phil *phil, int i)
{
	if (lock)
	{

	}
	else
	{
	
	}
}

int		take_drop_fork(int take, t_phil *phil)
{
	printf("FORK\n");
	int i;

	i = phil->index;
	if(take)
	{
		mutex_fork(take, phil, i);
		pthread_mutex_lock(&phil->info->fork[i]);
		printf("%ld %d has taken a fork\n", get_time(phil->info), i);
		if (i != phil->info->nb_philo - 1)
		{
			pthread_mutex_lock(&phil->info->fork[i + 1]);
			printf("%ld %d has taken a fork right\n", get_time(phil->info), i);
		}
		else
		{
			pthread_mutex_lock(&phil->info->fork[0]);	
			printf("%ld %d has taken a fork right\n", get_time(phil->info), i);
		}
	}
	else
	{
		pthread_mutex_unlock(&phil->info->fork[i]);
		printf("%d has drop fork\n", i);
		if (i != phil->info->nb_philo - 1)
		{
			pthread_mutex_unlock(&phil->info->fork[i + 1]);
			printf("%d has drop fork right\n", i);
		}
		else
		{
			pthread_mutex_unlock(&phil->info->fork[0]);
			printf("%d has drop fork right\n", i);
		}
	}
}
/*
int		take_drop_fork(int take, t_phil *phil)
{
	printf("FORK\n");
	int i;

	i = phil->index;
	if(take)
	{
		mutex_fork(take, phil, i)
		pthread_mutex_lock(&phil->info->fork[i]);
		printf("%ld %d has taken a fork\n", get_time(phil->info), i);
		if (i != phil->info->nb_philo - 1)
		{
			pthread_mutex_lock(&phil->info->fork[i + 1]);
			printf("%ld %d has taken a fork right\n", get_time(phil->info), i);
		}
		else
		{
			pthread_mutex_lock(&phil->info->fork[0]);	
			printf("%ld %d has taken a fork right\n", get_time(phil->info), i);
		}
	}
	else
	{
		pthread_mutex_unlock(&phil->info->fork[i]);
		printf("%d has drop fork\n", i);
		if (i != phil->info->nb_philo - 1)
		{
			pthread_mutex_unlock(&phil->info->fork[i + 1]);
			printf("%d has drop fork right\n", i);
		}
		else
		{
			pthread_mutex_unlock(&phil->info->fork[0]);
			printf("%d has drop fork right\n", i);
		}
	}
}
*/
void	 *routine(void *ptr)
{
	printf("ROUTINE\n");
	t_phil *phil = ptr;
	int i;
	i =  phil->index;
	while (phil->count_eat < phil->info->nb_eat && !phil->info->tue_la_mif_frere)
	{
		if(!phil->info->tue_la_mif_frere)
			take_drop_fork(1, phil);
		if(!phil->info->tue_la_mif_frere)
		{
			phil->last_eat = get_time(phil->info);
			printf("%ld %d is eating\n",get_time(phil->info), i);
			usleep(phil->info->time_eat * 1000);
			phil->count_eat++;
		}
			take_drop_fork(0, phil);
		if(!phil->info->tue_la_mif_frere)
		{
			printf("%ld %d is sleeping\n", get_time(phil->info), i);
			usleep(phil->info->time_sleep * 1000);
		}
		if(!phil->info->tue_la_mif_frere)
			printf("%ld %d is thinking\n", get_time(phil->info), i);
		printf("HERE %d\n", i);
	}
	printf("LEAVING routine %d\n", i);
	phil->finish = 1;
}

int check_data(char *argv, int index)
{
	int i;

	i = 0;
	if (index != 5)
	{
		while(argv[i])
		{
			if (!ft_isdigit(argv[i]))
				return (0);
			i++;
		}
		return (1);
	}
	else
	{
		if (argv[0] != '[' || argv[ft_strlen(argv) - 1] != ']')
			return (0);
		while (++i < ft_strlen(argv) - 1)
			if (!ft_isdigit(argv[i]))
				return (0);
		return (1);
	}
}

int		init(t_info *info, t_phil **phil)
{
	printf("INIT\n");
	int i;
		
	i = 0;
	*phil = malloc(sizeof(t_phil) * info->nb_philo);
	info->fork = malloc(sizeof(pthread_mutex_t) * info->nb_philo);
	info->tue_la_mif_frere = 0;
	gettimeofday(&info->start, NULL);
	while (i < info->nb_philo)
	{
		pthread_mutex_init(&info->fork[i], NULL);
		i++;
	}

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
	return(0);
}

int		destroy(t_info *info, t_phil **phil)
{
	printf("DESTROY LA FAMILLE \n");
	int i;
	
	i = 0;
	while (i < info->nb_philo)
	{
		printf("JOIN %d\n", i);
		pthread_join((*phil)[i].philo, NULL);
		i++;
	}

	i = 0;
	while (i < info->nb_philo)
	{
		printf("MUTEX DESTROY %d\n", i);
		pthread_mutex_destroy(&info->fork[i]);
		i++;
	}
	printf("LEAVE DESTROY\n");
	return (0);
}

int		parse(t_info *info, char **argv)
{
	printf("PARSE\n");
	int i;

	i = 1;
	while (i < 6 )
	{
		if (!check_data(argv[i], i))
		{
			printf("parsing error\n");
			exit_philo(NULL, NULL, 1);
		}
		i++;
	}
	info->nb_philo = ft_atoi(argv[1]);
	info->time_die = ft_atoi(argv[2]);	
	info->time_eat = ft_atoi(argv[3]);	
	info->time_sleep = ft_atoi(argv[4]);	
	info->nb_eat = ft_atoi(argv[5] + 1);
	return(0);
}

int		checker(t_info *info, t_phil **phil)
{
	printf("CHECKER\n");
	int i;
	int finish;

	i = 0;
	finish = 1;
	while (i < info->nb_philo)
	{
		if((*phil)[i].finish == 0)
			finish = 0;
		
		printf("=====time %d (%ld - %ld)= %ld\n", i,get_time(info), (*phil)[i].last_eat, get_time(info) - (*phil)[i].last_eat);
		if(get_time(info) - (*phil)[i].last_eat >= info->time_die)
		{
			printf("=====%ld %d DIED\n", get_time(info), i);
			exit_philo(info, phil, 0);
		}
		i++;
	}
	if (finish)
	{
		printf("FINISH OPERATION\n");
		exit_philo(info, phil, 1);
	}
	return(0);
}

int		philo(t_info *info, t_phil **phil)
{
	printf("PHILO\n");
	int i;

	i = 0;
	while (i < info->nb_philo)
	{
		pthread_create(&(*phil)[i].philo, NULL, &routine,(void *)&(*phil)[i]);
		i++;
	}
	while (1)
	{
		checker(info, phil);
	}
	return (0);
}

int 	main(int argc, char **argv)
{
	printf("MAIN\n");
	t_info info;
	t_phil *phil;
	
	if (argc == 6)
	{
		parse(&info, argv);
		init(&info, &phil);
		philo(&info, &phil);
		destroy(&info, &phil);
	}
	else
		printf("wrong number of arguments\n");
	return(0);
}

/*philo avec une seule fork
at least nb de repas
decouper les usleep
0 philo
1philo
limiter a 200 philo
ne rien ecrire apres la mort
race condition , fsanitize=thread

dans chaque philo tu peux mettre : usleep ( time_to_eat * (id % 2) / 2) juste apres les avoir lancé

vue qu'ils sont legerement lancer en decaler, ils vont essayer de prendre une puis deux fourchette et ensuite essayer de manger si ils ont deux fourchettes , mais si ils n'ont pas 2 fourchettes tu libre la fourchettes qu'il a
*/
