/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:31:20 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 12:31:26 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_cmd_block	*get_cmd_block(void)
{
	static t_cmd_block	*cmd = NULL;
	t_gc				*gc;

	gc = get_gc();
	if (cmd == NULL)
		cmd = init_command_struct(gc);
	return (cmd);
}
