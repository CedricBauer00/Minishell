/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/09 13:17:20 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void	execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	if (cur && cur->io_streams && cur->io_streams->heredoc_eof)
	{
		if (heredoc_fd_offset_and_redir(cur) == -1)
		{
			perror("errror heredoc");
			gc_free(gc);
			exit(1);
		}
	}
	processing_pipe(cur);
	set_io_streams(cur);
	if (cur && cur->built_in)
	{
		// fprintf(stderr, RED"is it in pipe for execute_builtin execute_child()\n"DEFAULT);
		execute_builtin(cur, shell);
	}
	if (cur && cur->args)
	{
		//fprintf(stderr, RED"is it in pipe for external cmd execute_child()\n"DEFAULT);
		run_execve(cur, gc);
	}
}

void	fork_and_execute(t_cmd_block *cmd_block, t_gc *gc, int *i)
{
	t_cmd_block	*cur;
	pid_t		pid;
	t_shell		*shell;

	shell = get_shell();
	cur = cmd_block;
	if (cur->next)
	{
		add_pipe(&cur);
		// fprintf(stderr, YELLOW"[pid %d] p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT,getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
	}
	pid = fork();
	// fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
	if (pid == 0) //signal function call?
		execute_child(cur, gc, shell);
	close_pipefd(cur);
	shell->pids[*i] = pid;
}
