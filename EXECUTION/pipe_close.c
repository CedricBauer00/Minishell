/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:42:19 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/16 10:29:05 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_first_pipefd(t_cmd_block *cmd)
{
	if (cmd->pipe->pipefd[1] >= 0)
	{
		close(cmd->pipe->pipefd[1]);
		fprintf(stderr, "[pid : %d], close(%d)\n",getpid(), cmd->pipe->pipefd[1]);
		cmd->pipe->pipefd[1] = -1;
	}
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	// if (cmd->pipe->pipefd[0])
	// 	fprintf(stderr, "in parent close () first pipe read %d\n",cmd->pipe->pipefd[0]);
	// if (cmd->pipe->pipefd[1])
	// 	fprintf(stderr, "in parent close () first pipe write %d\n",cmd->pipe->pipefd[1]);
}

void	close_middle_pipefd(t_cmd_block *cmd)
{
	if (cmd->prev_read_end_fd >= 0)
	{
		close(cmd->prev_read_end_fd);
		fprintf(stderr, "[pid : %d], close(%d)\n", getpid(), cmd->prev_read_end_fd);
		cmd->prev_read_end_fd = -1;
	}
	// if (cmd->pipe->pipefd[0] >= 0)
	// {
	// 	close(cmd->pipe->pipefd[0]);
	// 	fprintf(stderr, "[pid : %d], close(%d)\n",getpid(), cmd->pipe->pipefd[0]);
	// 	cmd->pipe->pipefd[0] = -1;
	// }
	if (cmd->pipe->pipefd[1] >= 0)
	{
		close(cmd->pipe->pipefd[1]);
		fprintf(stderr, "[pid : %d], close(%d)\n",getpid(), cmd->pipe->pipefd[1]);
		cmd->pipe->pipefd[1] = -1;
	}
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	
	// if (cmd->pipe->pipefd[0])
	// 	fprintf(stderr, "in parent close () middle pipe read %d\n", cmd->pipe->pipefd[0]);
	// if (cmd->pipe->pipefd[1])
	// 	fprintf(stderr, "in parent close () middle pipe write %d\n", cmd->pipe->pipefd[1]);
}

void	close_last_pipefd(t_cmd_block *cmd)
{
	if (cmd->prev_read_end_fd >= 0)
	{
		close(cmd->prev_read_end_fd);
		fprintf(stderr, "[pid : %d], close(%d)\n",getpid(), cmd->prev_read_end_fd);
		cmd->prev_read_end_fd = -1;
	}
}

void	close_pipefd(t_cmd_block *cmd)
{
	if (!cmd)
		return ;
	if (!cmd->prev && cmd->next)
	{
		fprintf(stderr, "parent proc try to close first pipe cmd \n");
		close_first_pipefd(cmd);
	}
	else if (cmd->prev && cmd->next)
	{
		fprintf(stderr, "parent proc try to close middle pipe cmd \n");
		close_middle_pipefd(cmd);
	}
	else if (!cmd->next)
	{
		fprintf(stderr, "parent proc try to close last pipe cmd \n");
		close_last_pipefd(cmd);
	}
}
