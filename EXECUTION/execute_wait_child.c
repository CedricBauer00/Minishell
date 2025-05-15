/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_wait_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/15 14:41:54 by cbauer           ###   ########.fr       */
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
	if (cur && cur->io_streams && cur->io_streams->heredoc_eof)
	{
		if (heredoc_fd_offset_and_redir(cur) == -1)
		{
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

static void check_open_fds(void)
{
    int fd;
    for (fd = 0; fd < 1024; fd++) {
        if (fcntl(fd, F_GETFD) != -1)
            fprintf(stderr, "FD %d is open\n", fd);
    }
}

void	execute_pipeline(t_cmd_block *cmd_block)
{
	t_cmd_block	*cur;
	t_gc		*gc;
	int			i;

	i = 0;
	gc = get_gc();
	cur = cmd_block;
	if (!cur->next)
		return ;
	while (cur)
	{
		fprintf(stderr, "cur %p\n", cur);
		fork_and_execute(cur, gc, &i);
		i++;
		cur = cur->next;
	}
	fprintf(stderr, "procs counts : %d\n", i);
	wait_for_child_and_update_status(i);
}

void	fork_and_execute(t_cmd_block *cur, t_gc *gc, int *i)
{
	pid_t		pid;
	t_shell		*shell;

	shell = get_shell();
	if (cur && cur->next)
			{
				add_pipe(&cur);
				fprintf(stderr, "[pid %d] pipe ( %d, %d ) \n",getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
			}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	fprintf(stderr, "fork()\n");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(shell->stdin_backup);
		close(shell->stdout_backup);
		execute_child(cur, gc, shell);
		check_open_fds();
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
}
