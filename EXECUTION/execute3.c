/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/11 13:17:59 by cbauer           ###   ########.fr       */
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
		execute_builtin(cur, shell);
		exit(shell->last_status_exit);
	}
	if (cur && cur->args && cur->args[0] && !cur->built_in)
	{
		run_execve(cur, gc);
	}
}

void	fork_and_execute(t_cmd_block *cur, t_gc *gc, int *i)
{
	pid_t		pid;
	t_shell		*shell;

	shell = get_shell();
	if (cur && cur->next)
	{
		add_pipe(&cur);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_child(cur, gc, shell);
	}
	close_pipefd(cur);
	shell->pids[*i] = pid;
}

