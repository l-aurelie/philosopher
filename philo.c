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
	pthread_mutex_t print_turn;
	pthread_mutex_t check_off;
	int turn_off;
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
	pthread_mutex_t check_meal;
	t_info *info;
}				t_phil;

int		destroy(t_info *info, t_phil **phil);
long 	get_time(t_info *info);

int		check_turn_off(t_info *info)
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

int		ft_usleep(int time_to_sleep, t_info *info)
{
	long start;
	
	start = get_time(info);
	while (get_time(info) - start < time_to_sleep && !check_turn_off(info))
	{
		usleep(100);
		if(check_turn_off(info) && get_time(info) - start < time_to_sleep )//DEBUG
			return 1;
	}
	return (0);
}

int		print_state(char *state, t_phil *phil, t_info *info)
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

int		exit_philo(t_info *info, t_phil **phil, int failure)
{
	//printf("EXIT TA MERE\n");
	//free;

	pthread_mutex_lock(&info->check_off);
	info->turn_off = 1;
	pthread_mutex_unlock(&info->check_off);
	destroy(info, phil);
	if (failure)
		exit(1);
	exit(0);
	return (0);
}

long 	get_time(t_info *info)
{
	long time_sec;
	long time_msec;
	
	if (gettimeofday(&info->time, NULL))
		return (0);
	time_sec = info->time.tv_sec - info->start.tv_sec;
	//printf("sec => %ld - %ld = %ld\n", info->time.tv_sec, info->start.tv_sec, time_sec);
	time_msec = info->time.tv_usec / 1000 - info->start.tv_usec / 1000;
	//printf("msec => %d - %d = %ld\n", info->time.tv_usec / 1000, info->start.tv_usec /1000, time_msec);
	if (time_msec < 0)
	{
		time_msec += 1000;
		time_sec -= 1;
	}
	time_sec = (time_sec * 1000) + time_msec;
	//printf("GET TIME = %ld\n", time_sec);
	return (time_sec);
}

int		mutex_fork(int lock, t_phil *phil, int i)
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
		return(0);
	}
	return (0);
}

int		take_drop_fork(int take, t_phil *phil)
{
	int i;

	i = phil->index;
	mutex_fork(take, phil, i);
	if (i != phil->info->nb_philo - 1)
		mutex_fork(take, phil, i + 1);
	else
		mutex_fork(take, phil, 0);
	return (0);
}

void	 *routine(void *ptr)
{
	t_phil *phil = ptr;
	int i;
	i =  phil->index;
	if (i % 2 != 0)
		usleep(100);
	while (!check_turn_off(phil->info) && phil->info->nb_eat != 0)
	{
		//	printf("%ld ROUTINE %d (%d)\n",get_time(phil->info), i, phil->count_eat);
		//if(!check_turn_off(phil->info))
		print_state("is thinking\n", phil, phil->info);
		if(!check_turn_off(phil->info))
		{
			take_drop_fork(1, phil);
			phil->last_eat = get_time(phil->info);
			print_state("=====is eating\n", phil, phil->info);
			ft_usleep(phil->info->time_eat /** 1000*/, phil->info);
			//if(ft_usleep(phil->info->time_eat /** 1000*/, phil->info))
			//	printf("%ld %d AWAKE from eat\n", get_time(phil->info), i);//DEBUG
			phil->count_eat++;
			if (phil->count_eat == phil->info->nb_eat)
			{
				//printf("%ld %d -----has finish\n", get_time(phil->info), i);
				phil->finish = 1;
				usleep(20);	
			}
			take_drop_fork(0, phil);
		}
		//if(!check_turn_off(phil->info))
		//{
			print_state("is sleeping\n", phil, phil->info);
			ft_usleep(phil->info->time_sleep /** 1000*/, phil->info);
			//if (ft_usleep(phil->info->time_sleep /** 1000*/, phil->info))
			//	printf("%ld %dAWAKE from sleep\n", get_time(phil->info), i);//DEBUG
		//}
	}
	//printf("LEAVING routine %d (%d)\n", i, phil->count_eat);
	return (0);
}

int check_data(char *argv, int index)
{
	int i;

	i = 0;
	while(argv[i])
	{
		if (!ft_isdigit(argv[i]))
			return (0);
		i++;
		if (i > 10)
		{
			printf("enter an integer please\n");
			return (0);
		}
	}
	return (1);
}

int		init(t_info *info, t_phil **phil)
{
	//printf("INIT\n");
	int i;
		
	i = 0;
	*phil = malloc(sizeof(t_phil) * info->nb_philo);
	info->fork = malloc(sizeof(pthread_mutex_t) * info->nb_philo);
	info->turn_off = 0;
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
	//printf("DESTROY LA FAMILLE \n");
	int i;
	
	i = 0;
	while (i < info->nb_philo)
	{
	//	printf("JOIN %d\n", i);
		pthread_join((*phil)[i].philo, NULL);
		i++;
	}

	i = 0;
	while (i < info->nb_philo)
	{
	//	printf("MUTEX DESTROY %d\n", i);
		pthread_mutex_destroy(&info->fork[i]);
		i++;
	}
	//printf("LEAVE DESTROY\n");
	return (0);
}

int		parse(t_info *info, char **argv)
{
	int i;

	i = 1;
	while (argv[i])
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
	if (argv[5])
		info->nb_eat = ft_atoi(argv[5]);
	else 
		info->nb_eat = -1;
	return(0);
}

int		checker(t_info *info, t_phil **phil)
{
	int i;
	int finish;

	i = 0;
	finish = 1;
	while (i < info->nb_philo)
	{
		if((*phil)[i].finish == 0)
			finish = 0;
		
	//	printf("%d (%ld - %ld)= %ld\n", i,get_time(info), (*phil)[i].last_eat, get_time(info) - (*phil)[i].last_eat);
		if(get_time(info) - (*phil)[i].last_eat >= info->time_die)
		{
	//	printf("%d (%ld - %ld)= %ld\n", i,get_time(info), (*phil)[i].last_eat, get_time(info) - (*phil)[i].last_eat);
			printf("======%ldms %d DIED (last_eat -> %ld)\n", get_time(info), i, (*phil)[i].last_eat);
			exit_philo(info, phil, 0);
		}
		i++;
	}
	if (finish)
	{
		//printf("%ld FINISH OPERATION\n", get_time(info));
		exit_philo(info, phil, 1);
	}
	return(0);
}

int		philo(t_info *info, t_phil **phil)
{
	int i;

	i = 0;
	gettimeofday(&info->start, NULL);
	while (i < info->nb_philo)
	{
		if (!i % 2 == 0)
			usleep(100); 
		pthread_create(&(*phil)[i].philo, NULL, &routine,(void *)&(*phil)[i]);
		i++;
	}
	while (1)
	{
		usleep(500);
		checker(info, phil);
	}
	return (0);
}

int 	main(int argc, char **argv)
{
	//printf("MAIN\n");
	t_info info;
	t_phil *phil;
	
	if (argc == 6 || argc == 5)
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
