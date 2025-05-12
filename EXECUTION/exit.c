/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:53:04 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 14:39:24 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// static int over_and_under_check(const char *str, int *digit, int num, long long *exitvalue)
// {
// 	if (num > (LONGLONGMAX / 10) || (num == (LONGLONGMAX / 10)
// 		&& digit > LONGLONGMAX % 10))
// 	{
// 		*exitvalue = 255;
// 		return (-1);
// 	}
// 	if (num < (LONGLONGMIN / 10) || (num == (LONGLONGMIN / 10)
// 		&& digit > -(LONGLONGMIN % 10)))
// 	{
// 		*exitvalue = 255;
// 		return (-1);
// 	}
// }

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
			return (-1);
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
	*exitvalue = num * sign;
	return (0);
}

void	ft_exit(char **args, t_shell *shell, t_gc *gc)
{
	long long	exitvalue;

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
	if ((ft_atoll(args[0], &exitvalue) == -1))
	{
		printf("exit: %s numeric argument required\n", args[0]);
		gc_free(gc);
		exit(255);
	}
	shell->last_status_exit = (unsigned char)exitvalue % 256;
	exit(shell->last_status_exit);
	gc_free(gc);
}
