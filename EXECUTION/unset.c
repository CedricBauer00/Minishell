/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:31 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/16 16:16:52 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	foo(char *arg, t_shell *shell)
{
	t_gc		*gc;
	int			found;
	t_gc_list	*find;

	found = -1;
	find = NULL;
	gc = get_gc();
	if (!arg)
		return ;
	found = check_existing(shell->my_envp, arg);
	if (found >= 0)
	{
		find = find_node(gc->shell, (char *)shell->my_envp[found]);
		delete_node(&gc->shell, find);
		while (shell->my_envp[found + 1])
		{
			shell->my_envp[found] = shell->my_envp[found + 1];
			found++;
		}
		shell->my_envp[found] = NULL;
	}
}

void	ft_unset(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (!args || !shell)
		return ;
	while (args[i])
	{
		if (!is_valid_identifier(args[i], "unset"))
		{
			printf("unset : %s non valid identifier\n", args[i]);
			i++;
			continue ;
		}
		foo(args[i], shell);
		i++;
	}
}
