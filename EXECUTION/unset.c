/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:31 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 18:03:24 by jisokim2         ###   ########.fr       */
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
