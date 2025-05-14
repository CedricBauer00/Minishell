/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:42:19 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 16:50:57 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_first_pipefd(t_cmd_block *cmd)
{
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	if (cmd->pipe->pipefd[1] >= 0)
		close(cmd->pipe->pipefd[1]);
}

void	close_middle_pipefd(t_cmd_block *cmd)
{
	if (cmd->prev_read_end_fd >= 0)
		close(cmd->prev_read_end_fd);
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	if (cmd->pipe->pipefd[1] >= 0)
		close(cmd->pipe->pipefd[1]);
}

void	close_last_pipefd(t_cmd_block *cmd)
{
	if (cmd->prev->prev_read_end_fd >= 0)
		close(cmd->prev->prev_read_end_fd);
}

void	close_pipefd(t_cmd_block *cmd)
{
	if (!cmd || !cmd->pipe || !cmd->pipe->pipefd)
		return ;
	if (!cmd->prev && cmd->next)
		close_first_pipefd(cmd);
	else if (cmd->prev && cmd->next)
		close_middle_pipefd(cmd);
	else if (cmd->prev && !cmd->next)
		close_last_pipefd(cmd);
}
