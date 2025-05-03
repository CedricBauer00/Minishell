/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:53:59 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 17:03:20 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	print_envp(t_shell *shell, char **args)
{
	int		i;
	bool	is_path;
	int		j;
	t_gc	*gc;

	(void)args;
	i = 0;
	j = 0;
	is_path = false;
	if (!shell)
		return ;
	gc = get_gc();
	is_path = check_existing(shell->my_envp, "PATH");
	while(shell->my_envp[i])
	{
		// if (strcmp(args[0], "export") == 0)
		printf(YELLOW"declare -x %s\n"DEFAULT, shell->my_envp[i]);
		// if (strcmp(args[0], "env") == 0)
		// {
		// 	if (is_path)
		// 	{
		// 		while (shell->my_envp[j] != NULL)
		// 		{
		// 			if(ft_strchr(shell->my_envp[j], '=') != NULL)
		// 			{
		// 				printf(BLUE"%s\n"DEFAULT, shell->my_envp[j]);
		// 			}
		// 			j++;
		// 		}
		// 	}
		// 	else
		// 	{
		// 		gc_free(gc);
		// 		printf(RED"env: No such file or directory\n"DEFAULT);
		// 		exit(1);
		// 	}
		// }
		i++;
	}
}

void	export(char **args, t_shell *shell)
{
	t_gc *gc;

	gc = get_gc();
	if (!args[0])
		print_envp(shell, args);
	else if (args[1])
	{
		char	*name = extract_name(args[1]);
		is_exited(name, gc);
		printf("name:%s\n", name);
		char	*value = extract_value(args[1]);
		is_exited(value, gc);
		printf("value:%s\n", value);
		ft_setenv(name, value, 1 , shell);
		free(name);
		free(value);
	}
}

char	*extract_name(char *arg)
{
	int	i;

	i = 0;
	while(arg[i])
	{
		if (arg[i] == '=')
			break;
		i++;
	}
	return ft_substr(arg, 0, i);
}

char	*extract_value(char *arg)
{
	int	i;

	i = 0;
	while(arg[i] != '\0' && arg[i] != '=')
		i++;
	if (arg[i] == '=')
		return ft_strdup(arg + i + 1);
	else
		return NULL;
}
