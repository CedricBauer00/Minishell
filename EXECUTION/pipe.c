/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:18 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 12:20:07 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	add_pipe(t_cmd_block **cmd_block)
{
	t_pipe	*new_pipe_node;
	t_gc	*gc;

	gc = get_gc();
	if (!cmd_block || !*cmd_block)
		return ;
	new_pipe_node = init_pipe(gc);
	if (!new_pipe_node)
	{
		gc_free(gc);
		exit(1);
	}
	if ((*cmd_block)->pipe == NULL)
		(*cmd_block)->pipe = new_pipe_node;
}

bool	is_first_pipe(t_cmd_block *cmd)
{
	if (!cmd->prev && cmd->next)
		return (true);
	else
		return (false);
}

bool	is_middle_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && cmd->next)
		return (true);
	else
		return (false);
}

bool	is_last_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && !cmd->next)
		return (true);
	else
		return (false);
}

int	first_pipe_cmd(t_cmd_block *command)
{
	close(command->pipe->pipefd[0]);
	if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror(RED"first cmd dup2 error\n"DEFAULT);
		return (-1);
	}
	close(command->pipe->pipefd[1]);
	return (1);
}

int	middle_pipe_cmd(t_cmd_block *command)
{
	if (dup2(command->prev_read_end_fd, STDIN_FILENO) == -1)
		perror(RED"SE dup2 ERROR\n"DEFAULT);
	close(command->prev_read_end_fd);
	close(command->pipe->pipefd[0]);
	if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
		perror(RED"SE dup2 ERROR\n"DEFAULT);
	close(command->pipe->pipefd[1]);
	return (1);
}

int	last_pipe_cmd(t_cmd_block *command)
{
	if (dup2(command->prev_read_end_fd, STDIN_FILENO) == -1)
		close(command->prev_read_end_fd);
	return (1);
}

void	close_first_pipefd(t_cmd_block *cmd)
{
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	close(cmd->pipe->pipefd[1]);
}

void	close_middle_pipefd(t_cmd_block *cmd)
{
	close(cmd->prev_read_end_fd);
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	close(cmd->pipe->pipefd[1]);
}

void	close_last_pipefd(t_cmd_block *cmd)
{
	close(cmd->prev->prev_read_end_fd);
}

void	close_pipefd(t_cmd_block *cmd)
{
	if (!cmd->prev && cmd->next)
		close_first_pipefd(cmd);
	else if (cmd->prev && cmd->next)
		close_middle_pipefd(cmd);
	else if (cmd->prev && !cmd->next)
		close_last_pipefd(cmd);
}

void	processing_pipe(t_cmd_block *cmd)
{
	if (is_first_pipe(cmd))
		first_pipe_cmd(cmd);
	else if (is_middle_pipe(cmd))
		middle_pipe_cmd(cmd);
	else if (is_last_pipe(cmd))
		last_pipe_cmd(cmd);
}
