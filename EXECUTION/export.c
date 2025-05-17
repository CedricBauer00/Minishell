/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:04 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/17 12:43:30 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	export_append_case(char *name, char *value,
	t_shell *shell, t_gc *gc)
{
	char	*base_name;
	char	*old_value;
	char	*new_value;

	new_value = NULL;
	base_name = NULL;
	old_value = NULL;
	base_name = gc_substr(name, 0, ft_strlen(name) - 1, gc);
	old_value = find_var_in_env(shell->my_envp,
			base_name, ft_strlen(base_name));
	if (!old_value)
		ft_setenv(base_name, value, 1, shell);
	else
	{
		new_value = gc_strjoin(old_value, value, &gc->temp);
		ft_setenv(base_name, new_value, 1, shell);
	}
	return (1);
}

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
	if (name[ft_strlen(name) - 1] == '+')
	{
		if (export_append_case(name, value, shell, gc) == 1)
			return ;
	}
	ft_setenv(name, value, 1, shell);
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

char	*extract_name(char *arg)
{
	int		i;
	t_gc	*gc;

	gc = get_gc();
	i = 0;
	while (arg[i])
	{
		if (arg[0] == '+')
			break ;
		if (arg[i] == '=')
			break ;
		if (arg[i] == '+' && arg[1 + 1] == '=')
			break ;
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
