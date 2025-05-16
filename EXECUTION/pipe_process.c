/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:42:56 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/16 13:39:47 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	processing_pipe(t_cmd_block *cmd)
{
	if (is_first_pipe(cmd))
		first_pipe_cmd(cmd);
	else if (is_middle_pipe(cmd))
		middle_pipe_cmd(cmd);
	else if (is_last_pipe(cmd))
		last_pipe_cmd(cmd);
}

int	first_pipe_cmd(t_cmd_block *command)
{
	if (command->pipe->pipefd[0] >= 0)
	{
		close(command->pipe->pipefd[0]);
		fprintf(stderr, YELLOW"[pid : %d] close(%d)\n"DEFAULT,getpid(), command->pipe->pipefd[0]);
	}
	if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
		return (-1);
	if (command->pipe->pipefd[1] >= 0)
	{
		close(command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW"[pid : %d] close(%d)\n"DEFAULT,getpid(), command->pipe->pipefd[1]);
	}
	return (1);
}

int	middle_pipe_cmd(t_cmd_block *command)
{
	if (dup2(command->prev_read_end_fd, STDIN_FILENO) == -1)
		return (-1);
	if (command->prev_read_end_fd >= 0)
	{
		close(command->prev_read_end_fd);
		fprintf(stderr, YELLOW"[pid : %d] close(%d)\n"DEFAULT, getpid(), command->prev_read_end_fd);
	}
	if (command->pipe->pipefd[0] >= 0)
	{
		close(command->pipe->pipefd[0]);
		fprintf(stderr, YELLOW"[pid : %d] close(%d)\n"DEFAULT,getpid(), command->pipe->pipefd[0]);
	}
	if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
		return (-1);
	if (command->pipe->pipefd[1] >= 0)
	{
		close(command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW"[pid : %d] close(%d)\n"DEFAULT,getpid(), command->pipe->pipefd[1]);
	}
	return (1);
}

int	last_pipe_cmd(t_cmd_block *command)
{
	if (dup2(command->prev_read_end_fd, STDIN_FILENO) == -1)
		return (-1);
	if (command->prev_read_end_fd >= 0)
	{
		close(command->prev_read_end_fd);
		fprintf(stderr, YELLOW"[pid : %d] close(%d)\n"DEFAULT, getpid(), command->prev_read_end_fd);
	}
	return (1);
}
