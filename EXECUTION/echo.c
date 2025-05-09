/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:39 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/09 15:32:32 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_echo(char **args, bool is_newline, int i, int j)
{
	while (args[i] && args[i][0] ==  '-')
	{
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0' && args[i][j - 1] == 'n')
		{
			is_newline = false;
			i++;
		}
		else
			break ;
	}
	if (args[i])
	{
		while (args[i] != NULL)
		{
			printf("%s", args[i]);
			if (args[i + 1] != NULL)
				printf(" ");
			i++;
		}
	}
	if (is_newline)
		printf("\n");
}


