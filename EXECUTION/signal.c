/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:56:54 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/07 10:03:25 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	signal_handler_for_child(int sign)
{
	t_gc	*gc;

	gc = get_gc();
	if (sign == SIGINT)
	{
		fprintf(stderr, RED"signal SIGINT heredoc\n"DEFAULT);
		gc_free(gc);
	}
	if (sign == SIGQUIT)
	{
		fprintf(stderr, RED"signal SIGQUIT\n"DEFAULT);
		gc_free(gc);
		exit(1);
	}
}

