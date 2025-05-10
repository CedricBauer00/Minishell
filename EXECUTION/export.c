/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:04 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 14:59:37 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	export_handling(char *arg, t_shell *shell)
{
	char *name;
	char *value;
	bool check;
	t_gc *gc;

	gc = get_gc();
	check = false;
	name = extract_name(arg);
	is_exited(name, gc);
	check = is_valid_identifier(name);
	if (check == false)
	{
		printf(RED"non valid identifier\n"DEFAULT);
		return ;
	}
	value = extract_value(arg);
	ft_setenv(name, value, 1 , shell);
}

void	print_envp(t_shell *shell)
{
	int	i;
	char *name;
	char *value;
	
	i = 0;
	if (!shell)
		return ;

	while(shell->my_envp[i])
	{
		name = extract_name(shell->my_envp[i]);
		value = extract_value(shell->my_envp[i]);
		if (ft_strchr(shell->my_envp[i] ,'='))
			printf(YELLOW"declare -x %s=\"%s\"\n"DEFAULT, name, value);
		else if (name)
			printf(YELLOW"declare -x %s\n"DEFAULT, name);
		i++;
	}
}

void	export(char **args, t_shell *shell)
{
	int		i;

	i = 0;
	if (args && args[0] == NULL)
		print_envp(shell);
	while ((args[i]))
	{
		export_handling(args[i], shell);
		i++;
	}
}

char	*extract_name(char *arg)
{
	int	i;
	t_gc *gc;

	gc = get_gc();
	i = 0;
	while(arg[i])
	{
		if (arg[i] == '=')
			break;
		i++;
	}
	return gc_substr(arg, 0, i, gc);
}

char	*extract_value(char *arg)
{
	t_gc *gc;
	int	i;

	gc = get_gc();
	i = 0;
	while(arg[i] != '\0' && arg[i] != '=')
	{
		i++;
	}
	if (arg[i] == '=')
		return gc_strdup(arg + i + 1, &gc->temp);
	else
		return NULL;
}
