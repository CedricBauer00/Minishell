/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:10:51 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 17:10:53 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*find_var_in_env(char **my_envp, char *find, size_t find_len, t_gc_list *gc_lst)
{
	int	i;
	char *result;

	i = 0;
	while(my_envp[i])
	{
		if (strncmp(my_envp[i], find, find_len) == 0 && my_envp[i][find_len] == '=')
		{
			result = my_envp[i] + find_len + 1;
			return gc_strdup(result, &gc_lst);
		}
		i++;
	}
	return NULL;
}