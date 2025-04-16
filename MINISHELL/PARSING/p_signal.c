/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:26:29 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/15 13:56:49 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	signal_func(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
// signals im Parentprozess (minishell) == ignore 
// signals im Childprozess == SIG_DFL