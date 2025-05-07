/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:18 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 10:01:36 by cbauer           ###   ########.fr       */
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
		return true;
	else
		return false;
}

bool	is_middle_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && cmd->next)
		return true;
	else
		return false;
}

bool	is_last_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && !cmd->next)
		return true;
	else
		return false;
}

int	first_pipe_cmd(t_cmd_block *command)
{
	fprintf(stderr,BLUE"first_pipe getpid() %d\n"DEFAULT, getpid());
	close(command->pipe->pipefd[0]);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[0]);
	if (dup2( command->pipe->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror(RED"first cmd dup2 error\n"DEFAULT);
		return (-1);
	}
	fprintf(stderr, YELLOW "FIRST PIPE[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->pipefd[1], STDOUT_FILENO);
	close( command->pipe->pipefd[1]);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(),  command->pipe->pipefd[1]);
	fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	return (1);
}

int	middle_pipe_cmd(t_cmd_block *command)
{
	fprintf(stderr,BLUE"second_pipe\n"DEFAULT);
	if (dup2(command->prev_read_end_fd , STDIN_FILENO) == -1)
		perror(RED"SE dup2 ERROR\n"DEFAULT);
	fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->prev_read_end_fd , STDIN_FILENO);
	fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	close(command->prev_read_end_fd);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->prev_read_end_fd );
	close(command->pipe->pipefd[0]);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[0]);
	if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
		perror(RED"SE dup2 ERROR\n"DEFAULT);
	fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->pipefd[1], STDOUT_FILENO);
	fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	close(command->pipe->pipefd[1]);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[1]);
	return 1;
}

int	last_pipe_cmd(t_cmd_block *command)
{
	fprintf(stderr,BLUE"last_pipe getpid() %d\n"DEFAULT, getpid());

	if (dup2(command->prev_read_end_fd, STDIN_FILENO) == -1)
		fprintf(stderr,RED"last_pipe_cmd() dup2()error\n"DEFAULT);
	fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->prev_read_end_fd, STDIN_FILENO);
	fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	close(command->prev_read_end_fd);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->prev_read_end_fd);
	return (1);
}

void	close_first_pipefd(t_cmd_block *cmd)
{
	fprintf(stderr,GREEN"in close_first_pipefd\n");
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	fprintf(stderr,GREEN"[pid %d] in close_first_pipefd, cmd->prev_read_end_fd : %d, cmd->cur_fd_write_end :%d\n"DEFAULT, getpid(),
		cmd->prev_read_end_fd, cmd->cur_fd_write_end);
	close(cmd->pipe->pipefd[1]);
	fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(),  cmd->pipe->pipefd[1]);
}

void	close_middle_pipefd(t_cmd_block *cmd)
{
	fprintf(stderr,GREEN"in close_middle_pipefd\n");
	close(cmd->prev_read_end_fd);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), cmd->prev_read_end_fd);
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	close(cmd->pipe->pipefd[1]);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), cmd->pipe->pipefd[1]);
}

void	close_last_pipefd(t_cmd_block *cmd)
{
	fprintf(stderr,GREEN"in close_last_pipefd()\n");
	close(cmd->prev->prev_read_end_fd);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), cmd->prev_read_end_fd);
	fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
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
