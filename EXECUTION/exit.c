/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:53:04 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 16:46:40 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	check_over_under_flow(long long num, int digit,
	long long *exitvalue)
{
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
	return (0);
}

static int	get_sign(const char **str)
{
	int	sign;

	sign = 1;
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign *= -1;
		(*str)++;
	}
	return (sign);
}

long long	ft_atoll(const char *str, long long *exitvalue)
{
	int			digit;
	long long	num;
	int			check;
	int			sign;

	sign = 1;
	num = 0;
	if (!str)
		return (0);
	sign = get_sign(&str);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		digit = *str - '0';
		check = check_over_under_flow(num, digit, exitvalue);
		if (check == -1)
			return (check);
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
