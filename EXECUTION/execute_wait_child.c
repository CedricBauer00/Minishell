/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_wait_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/17 13:56:55 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	prevent_zombie_process(void)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

void	execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	processing_pipe(cur);
	if (cur && cur->io_streams && cur->io_streams->heredoc_eof)
	{
		heredoc_fd_offset_and_redir(cur);
	}
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

	if (!cur)
		return ;
	shell = get_shell();
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(shell->stdin_backup);
		close(shell->stdout_backup);
		execute_child(cur, gc, shell);
		exit(0);
	}
	close_pipefd(cur);
	shell->pids[*i] = pid;
}

void	wait_for_child_and_update_status(int i)
{
	t_shell	*shell;
	int		status;
	int		idx;

	idx = 0;
	shell = get_shell();
	if (!shell->pids)
		return ;
	while (idx < i)
	{
		waitpid(shell->pids[idx], &status, 0);
		if (WIFEXITED(status))
		{
			shell->last_status_exit = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			close(shell->stdin_backup);
			close(shell->stdout_backup);
			shell->last_status_exit = 128 + WTERMSIG(status);
		}
		idx++;
	}
	if (shell->heredoc_fd)
		unlink("temp_heredoc");
}
