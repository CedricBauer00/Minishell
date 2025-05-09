/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:18 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/09 15:58:36 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*find_var_in_env(char **my_envp, char *find, size_t find_len, \
	t_gc_list *gc_lst)
{
	int		i;
	char	*result;

	i = 0;
	while (my_envp[i])
	{
		if (strncmp(my_envp[i], find, find_len) == 0
			&& my_envp[i][find_len] == '=')
		{
			result = my_envp[i] + find_len + 1;
			return (gc_strdup(result, &gc_lst));
		}
		i++;
	}
	return (NULL);
}

bool	is_valid_identifier(const char *name)
{
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (false);
	else
		return (true);
}

char	*create_new_path(const char *name, const char *value)
{
	int		namelen;
	int		total_len;
	t_gc	*gc;
	char	*new_path;

	namelen = ft_strlen(name);
	gc = get_gc();
	total_len = namelen + 1;

	if (value)
		total_len += ft_strlen(value) + 1;
	new_path = do_alloc(&gc->shell, total_len, TYPE_SINGLE_PTR, "new_path");
	if (!new_path)
	{
		perror("failed do_alloc new_path");
		gc_free(gc);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_path, name, namelen + 1);
	if (value)
	{
		new_path[namelen] = '=';
		ft_strlcpy(new_path + namelen + 1, value, total_len - namelen - 1);
	}
	return (new_path);
}

int	check_existing(char **my_envp, const char *name)
{
	int	i;

	i = 0;
	while (my_envp[i])
	{
		if ((strncmp(my_envp[i], name, strlen(name)) == 0)
			&& (my_envp[i][strlen(name)] == '='))
			return (i);
		i++;
	}
	return (-1);
}

int	get_env_count(char **my_envp)
{
	if (!my_envp)
		return (0);
	int	env_count;

	env_count = 0;
	while (my_envp[env_count])
		env_count++;
	return (env_count);
}
