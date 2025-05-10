/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 14:50:40 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

//memo : maybe i need t_cmd_block **cur
void	execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	//printf("run_execve() builtin %s, called with cmd_block->args[0]: %s\n", 
	//cur->built_in ,cur->args ? cur->args[0] : "NULL");
	//TODO i can change later if statsments  --> if (shell->heredoc_fd > 0)
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
		fprintf(stderr, RED"is it in pipe for execute_built in execute_child()\n"DEFAULT);
		execute_builtin(cur, shell);
		exit(shell->last_status_exit);
	}
	if (cur && cur->args && cur->args[0] && !cur->built_in)
	{
		fprintf(stderr, RED"is it in pipe for external cmd in execute_child()\n"DEFAULT);
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
		fprintf(stderr, YELLOW"[pid %d] p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT,getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
	}
	pid = fork();
	fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
	if (pid == 0)
		execute_child(cur, gc, shell);
	close_pipefd(cur);
	shell->pids[*i] = pid;
}

