/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:39 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/09 14:34:36 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_echo(char **args, t_shell *shell)
{
	int i  = 0;
	bool is_newline = true;
	t_gc	*gc;

	(void)shell;
	gc = get_gc();
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		is_newline = false;
		i++;
	}
	if (args[i])
	{
		while (args[i] != NULL)
		{
			printf(YELLOW"%s", args[i]);
			if (args[i + 1] != NULL)
				printf(" ");
			i++;
		}
	}
	if (is_newline)
		printf(YELLOW"\n"DEFAULT);
}


