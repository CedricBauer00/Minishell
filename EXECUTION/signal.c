/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:56:54 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/10 13:58:08 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	signal_handler_for_child(int sign)
{
	t_gc	*gc;

	gc = get_gc();
	if (sign == SIGINT)
		gc_free(gc);
	if (sign == SIGQUIT)
	{
		gc_free(gc);
		exit(1);
	}
}
