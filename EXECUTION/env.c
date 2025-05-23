/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:44 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/14 17:26:39 by jisokim2         ###   ########.fr       */
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
