/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:44 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 15:09:29 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_env(char **args, t_shell *shell)
{
	int		i;

	i = 0;
	if (*args)
	{
		printf(RED"ENV TOO MANY ARGS\n"DEFAULT);
		return ;
	}
	while(shell->my_envp[i])
	{
		if(extract_name(shell->my_envp[i]) && extract_value(shell->my_envp[i]))
			printf(YELLOW"%s=%s\n"DEFAULT, extract_name(shell->my_envp[i]), extract_value(shell->my_envp[i]));
		i++;
	}
}
