/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:26:29 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/10 15:46:32 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	signal_func(int sig)
{
	t_shell	*shell;

	shell = get_shell();
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	shell->last_status_exit = 1;
}

int	ttyattr(void)
{
	struct termios	temp;

	if (tcgetattr(0, &temp) < 0)
		return (-1);
	temp.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(0, TCSANOW, &temp) < 0)
		return (-1);
	return (0);
}
