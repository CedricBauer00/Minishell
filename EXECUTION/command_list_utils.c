/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:10:34 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 17:10:37 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//todo fix it where i use it? idk
// t_cmd_block *add_command(t_gc_list *gc_lst, char *cmd, char **args, t_token_type type)
// {
// 	t_cmd_block *new_cmd = init_command_struct(gc_lst);
// 	if (!new_cmd)
// 	{
// 		perror(RED"add_command()failed"DEFAULT);
// 		return NULL;
// 	}
// 	new_cmd->args = args;
// 	new_cmd->cmd = cmd;
// 	//new_cmd->type = type;
// 	return new_cmd;
// }