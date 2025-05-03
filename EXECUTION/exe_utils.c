/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:11:14 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 17:11:15 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	is_exited(void *failed, t_gc *gc)
{
	if (!failed)
	{
		perror(RED"in is_exited() making new_pipe in add_pipe()error\n"DEFAULT);
		gc_free(gc);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf(BLUE"in is_exited() alloc success\n"DEFAULT);
	}
}