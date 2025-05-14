/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:44 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/14 13:55:17 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_env(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (*args)
		return ;
	while (shell->my_envp[i])
	{
		if (extract_name(shell->my_envp[i]) && extract_value(shell->my_envp[i]))
			printf("%s=%s\n", extract_name(shell->my_envp[i]), \
				extract_value(shell->my_envp[i]));
		i++;
	}
}
