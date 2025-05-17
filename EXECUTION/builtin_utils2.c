/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:46:42 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/17 12:46:43 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

void	recover_stdin_out(t_shell *shell)
{
	if (shell->stdin_backup > 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup > 0)
		close(shell->stdout_backup);
}
