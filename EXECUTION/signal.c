/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:56:54 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/10 17:31:35 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	signal_handler_for_child(int sign)
{
	t_gc	*gc;

	gc = get_gc();
	if (sign == SIGINT)
	{
		//write(1, "\n", 1);
		//all_free(&gc->temp);
		signal(SIGINT, SIG_DFL);
	}
	if (sign == SIGQUIT)
	{
		gc_free(gc);
		exit(1);
	}
}
