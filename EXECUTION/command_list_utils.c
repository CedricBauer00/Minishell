/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:29 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/05 14:16:30 by jisokim2         ###   ########.fr       */
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