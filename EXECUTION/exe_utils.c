/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:50 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 14:57:29 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	is_exited(void *failed, t_gc *gc)
{
	if (!failed)
	{
		// printf(RED"%s in is_exited()is null \n"DEFAULT,(char*)failed);
		gc_free(gc);
		exit(EXIT_FAILURE);
	}
	// else
	// 	printf(BLUE"in is_exited() alloc success\n"DEFAULT);
}
