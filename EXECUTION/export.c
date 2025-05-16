/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:04 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/16 16:56:19 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"


// static bool	is_valid_identifier_for_export(const char *name)
// {
// 	int	i;

// 	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
// 		return (false);
// 	i = 1;
// 	while (name[i])
// 	{
	
// 		if (!(ft_isalnum(name[i]) || name[i] == '_'))
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }


static void	export_handling(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	bool	check;
	t_gc	*gc;

	gc = get_gc();
	check = false;
	name = extract_name(arg);
	is_exited(name, gc);
	check = is_valid_identifier(name, "export");
	if (check == false)
	{
		printf("non valid identifier\n");
		return ;
	}
	value = extract_value(arg);
	
	if	((name[ft_strlen(name) - 1] == '+'))
	{
		char *base_name = gc_substr(name, 0, ft_strlen(name) - 1, gc);
		char *old_value = find_var_in_env(shell->my_envp, base_name, ft_strlen(base_name));
		printf("old_value : %s\n", old_value);
		if (!old_value)
			ft_setenv(base_name, value, 1, shell);
		else
		{
			char *new_value = gc_strjoin(old_value, value, &gc->temp);
			ft_setenv(base_name, new_value, 1, shell);
		}
		return ;
	}
	ft_setenv(name, value, 1, shell);
}

void	print_envp(t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;

	value = NULL;
	i = 0;
	if (!shell)
		return ;
	while (shell->my_envp[i])
	{
		name = extract_name(shell->my_envp[i]);
		value = extract_value(shell->my_envp[i]);
		if (ft_strchr(shell->my_envp[i], '='))
			printf("declare -x %s=\"%s\"\n", name, value);
		else if (name)
			printf("declare -x %s\n", name);
		i++;
	}
}

void	export(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (args && args[0] == NULL)
		print_envp(shell);
	while ((args[i]))
	{
		export_handling(args[i], shell);
		i++;
	}
}

//todo export += case
char	*extract_name(char *arg)
{
	int		i;
	t_gc	*gc;

	gc = get_gc();
	i = 0;
	while (arg[i])
	{
		if (arg[0] == '+')
			break;
		if (arg[i] == '=')
			break ;
		if (arg[i] == '+' && arg[1 + 1] == '=')
			break;
		i++;
	}
	return (gc_substr(arg, 0, i, gc));
}

char	*extract_value(char *arg)
{
	t_gc	*gc;
	int		i;

	i = 0;
	gc = get_gc();
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	if (arg[i] == '=')
		return (gc_strdup(arg + i + 1, &gc->temp));
	else
		return (NULL);
}
