/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:18 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/12 11:25:43 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*find_var_in_env(char **my_envp, char *find, size_t find_len)
{
	int		i;
	char	*result;
	t_gc	*gc;

	gc = get_gc();
	i = 0;
	while (my_envp[i])
	{
		if (ft_strncmp(my_envp[i], find, find_len) == 0
			&& my_envp[i][find_len] == '=')
		{
			result = my_envp[i] + find_len + 1;
			return (gc_strdup(result, &gc->temp));
		}
		i++;
	}
	return (NULL);
}

bool	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (false);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (false);
		i++;
	}
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
		if ((ft_strncmp(my_envp[i], name, strlen(name)) == 0))
			return (i);
		i++;
	}
	return (-1);
}

int	get_env_count(char **my_envp)
{
	int	env_count;

	if (!my_envp)
		return (0);
	env_count = 0;
	while (my_envp[env_count])
		env_count++;
	return (env_count);
}
