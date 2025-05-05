/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:18 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/05 14:23:21 by cbauer           ###   ########.fr       */
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
	int	i;

	i = 0;
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (false);
	i = 1;
	while (name[i])
	{
		if (!ft_isalpha(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
