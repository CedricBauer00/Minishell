/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:31 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 10:03:42 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_unset(char **args, t_shell *shell)
{
	int		i;
	int		found;
	bool	check;

	check = false;
	i = 0;
	if (args[i] != NULL)
	{
		check = is_valid_identifier(args[i]);
		{
			if (check == false)
			{
				perror(RED"non valid identifier\n"DEFAULT);
				return ;
			}
		}
		while (args[i])
		{
			found = -1;
			found = check_existing(shell->my_envp, args[i]);
			printf(GREEN"in ft_unset() %d\n"DEFAULT, found);
			if (found != -1)
			{
				t_gc *gc = get_gc();
				t_gc_list *find = find_node(gc->shell, (char*)shell->my_envp[found]);
				delete_node(&gc->shell, find);
				while(shell->my_envp[found + 1])
				{
					shell->my_envp[found] = shell->my_envp[found + 1];
					found++;
				}
				shell->my_envp[found] = NULL;
			}
			i++;
		}
	}
}
