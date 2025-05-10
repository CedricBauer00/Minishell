/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:50 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 13:48:18 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	is_exited(void *failed, t_gc *gc)
{
	if (!failed)
	{
		gc_free(gc);
		exit(EXIT_FAILURE);
	}
}
