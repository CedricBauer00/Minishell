/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:24:30 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 16:53:36 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	echo_helper2(char **args, int i, bool is_newline)
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

int	echo_helper(char **args, t_shell *shell, int i, bool is_newline)
{
	t_gc	*gc;
	char	*var_name;
	char	*var;

	gc = get_gc();
	if (ft_strncmp(args[0], "$", 1) == 0)
	{
		if (*args + 1)
		{
			var_name = *args + 1;
			var = find_var_in_env(shell->my_envp, var_name, \
				ft_strlen(var_name), gc->temp);
			if (!var)
			{
				gc_free(gc);
				exit(1);
			}
			printf(YELLOW"%s\n"DEFAULT, var);
		}
	}
	else
		echo_helper2(args, i, is_newline);
	return (0);
}

void	ft_echo(char **args, t_shell *shell)
{
	int		i;
	bool	is_newline;

	i = 0;
	is_newline = true;
	if (!args[0])
	{
		printf("\n");
		return ;
	}
	if (echo_helper(args, shell, i, is_newline) == 1)
		return ;
	if (is_newline)
		printf("\n"DEFAULT);
}
