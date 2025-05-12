/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:53:04 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/11 15:19:07 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

long long	ft_atoll(const char *str, long long *exitvalue)
{
	int			sign;
	int			digit;
	long long	num;

	num = 0;
	sign = 1;
	if (!str)
		return (0);
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		digit = *str - '0';
		if (num > (LONGLONGMAX / 10) || (num == (LONGLONGMAX / 10)
				&& digit > LONGLONGMAX % 10))
		{
			*exitvalue = 255;
			return (-1);
		}
		if (num < (LONGLONGMIN / 10) || (num == (LONGLONGMIN / 10)
				&& digit > -(LONGLONGMIN % 10)))
		{
			*exitvalue = 255;
			return (-1);
		}
		num = num * 10 + digit;
		str++;
	}
	num *= sign;
	if (num > LONGLONGMAX || num < LONGLONGMIN)
	{
		*exitvalue = 255;
		return (-1);
	}
	return (num);
}

void	ft_exit(char **args, t_shell *shell)
{
	long long	exitvalue;
	char		*temp;
	t_gc		*gc;
	int			i;

	gc = get_gc();
	temp = NULL;
	exitvalue = 0;
	write(1, "exit\n", 5);
	if (args[0] == NULL)
	{
		exit(shell->last_status_exit);
		gc_free(gc);
	}
	if (args[1] != NULL)
	{
		printf("exit: too many arguments\n");
		shell->last_status_exit = 1;
		all_free(&gc->temp);
		return ;
	}
	temp = args[0];
	if (*temp == '-' || *temp == '+')
		temp++;
	i = 0;
	while (args[0][i])
	{
		if (!(args[0][i] >= '0' && args[0][i] <= '9') || (args[0][i] >= '0' && args[0][i] <= '9'))
			printf("exit: %s numeric argument required\n", args[0]);
		gc_free(gc);
		exit(255);
		i++;
	}
	if (ft_atoll(args[0], &exitvalue) == -1)
	{
		printf("exit: %s numeric argument required\n", args[0]);
		gc_free(gc);
		exit(255);
	}
	shell->last_status_exit = (unsigned char)exitvalue % 256;
	exit(shell->last_status_exit);
	gc_free(gc);
}

// int	ft_atoll_helper(const char *str, int digit, long long num, int sign)
// {
// 	write(1, "here!\n", 6);
// 	while (*str)
// 	{
// 		if (*str < '0' || *str > '9')
// 			return (0);
// 		digit = *str - '0';
// 		if (num > (LONGLONGMAX / 10) || (num == (LONGLONGMAX / 10)
// 				&& digit > LONGLONGMAX % 10))
// 		{
// 			printf("here!\n");
// 			return (-1);
// 		}
// 		if (num < (LONGLONGMIN / 10) || (num == (LONGLONGMIN / 10)
// 				&& digit > -(LONGLONGMIN % 10)))
// 		{
// 			printf("here2\n");
// 			// *exitvalue = 255;
// 			return (-1);
// 		}
// 		num = num * 10 + digit;
// 		str++;
// 	}
// 	num *= sign;
// 	if (num > LONGLONGMAX || num < LONGLONGMIN)
// 		return (-1);
// 	return (num);
// }

// long long	ft_atoll(const char *str, long long *exitvalue)
// {
// 	int			sign;
// 	long long	num;
// 	printf("here4!\n");

// 	num = 0;
// 	sign = 1;
// 	if (!str)
// 		return (0);
// 	while (*str == ' ' || (*str >= 9 && *str <= 13))
// 		str++;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			sign *= -1;
// 		str++;
// 	}
// 	num = ft_atoll_helper(str, 0, num, sign);
// 	if (num < 0)
// 	{
// 		printf("here3!\n");
// 		*exitvalue = 255;
// 		return (-1);
// 	}
// 	num *= sign;
// 	if (num > LONGLONGMAX || num < LONGLONGMIN)
// 	{
// 		*exitvalue = 255;
// 		return (-1);
// 	}
// 	return (num);
// }