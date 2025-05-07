/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:39 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 09:44:51 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_echo(char **args, t_shell *shell)
{
	int i  = 0;
	bool is_newline = true;
	t_gc	*gc;

	gc = get_gc();
	fprintf(stderr, RED"try to do 'echo' ft_echo() \n"DEFAULT);
	if (ft_strncmp(args[0], "$", 1) == 0)
	{
		if (args[1])
		{
			char *var_name = *args + 1;
			char *var = find_var_in_env(shell->my_envp, var_name, ft_strlen(var_name), gc->temp);
			if (!var)
			{
				// all_free(&gc->temp);???
				// is_newline = true;
				printf(YELLOW"\n"DEFAULT);
			}
		}
	}
	else if (args[i])
	{
		while (args[i] && strcmp(args[i], "-n") == 0)
		{
			is_newline = false;
			i++;
		}
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


