/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_signal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:26:29 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/02 15:06:33 by cbauer           ###   ########.fr       */
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
