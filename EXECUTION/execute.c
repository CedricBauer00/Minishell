/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/06 16:56:25 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void	wait_for_child_and_update_status(int i)
{
	t_shell	*shell;
	int		status;
	int		idx;

	idx = 0;
	shell = get_shell();
	if(!shell->pids)
		return;
	while(idx < i)
	{
		fprintf(stderr, RED"shell->pids[idx] %d\n"DEFAULT, shell->pids[idx]);
		pid_t child_pid = wait4(shell->pids[idx], &status, 0 ,NULL);
		fprintf(stderr ,RED"child_pid %d\n"DEFAULT, child_pid);
		fprintf(stderr ,BLUE"parent got this from wait4() child_pid : %d\n"DEFAULT, child_pid);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		else if (WIFSIGNALED(status))
		{
			fprintf(stderr, RED"terminated by signal %d (%s)\n" DEFAULT, WTERMSIG(status), strsignal(WTERMSIG(status)));
			shell->last_status_exit = 128 + WTERMSIG(status);
		}
		idx++;
	}
}

int heredoc_fd_offset_and_redir(t_cmd_block *cur)
{
	t_shell	*shell;

	shell = get_shell();
	if (!cur)
		return -1;
	shell->heredoc_fd = open("temp_heredoc" ,O_RDWR, 0644);
	if (shell->heredoc_fd < 0)
	{
		return -1;
	}
	if (dup2(shell->heredoc_fd, STDIN_FILENO) == -1)
	{
		close(shell->heredoc_fd);
		unlink("temp_heredoc");
		return -1;
	}
	close(shell->heredoc_fd);
	unlink("temp_heredoc");
	return 1;
}

void	execute_builtin(t_cmd_block *cur, t_shell *shell)
{
	t_gc *gc;

	gc = get_gc();
	fprintf(stderr, GREEN"execute_builtin()\n"DEFAULT);
	if (strcmp(cur->built_in, "cd") == 0)
	{
		fprintf(stderr, RED"try to do 'cd' execute_builtin\n"DEFAULT);
		cd(cur->args, shell, gc);
	}
	if (strcmp(cur->built_in, "echo") == 0)
	{
		ft_echo(cur->args, shell);
	}
	if (strcmp(cur->built_in, "export") == 0)
	{
		export(cur->args, shell);
	}
	if (strcmp(cur->built_in, "pwd") == 0)
	{
		ft_pwd(cur->args, gc);
	}
	if (strcmp(cur->built_in, "env") == 0)
	{
		ft_env(cur->args, shell);
	}
	if (strcmp(cur->built_in, "unset") == 0)
	{
		ft_unset(cur->args, shell);
	}
}

void	single_cmd_execute(t_cmd_block *cur, t_gc *gc)
{
	t_shell	*shell;
	pid_t	pid;

	if (!cur)
		return;
	shell = get_shell();

	if (cur->io_streams)
		set_io_streams(cur);
	if (cur->is_built_in)
	{
		if (cur->io_streams && cur->io_streams->heredoc_eof)
		{
			if (heredoc_fd_offset_and_redir(cur) == -1)
			{
				perror(RED"heredoc error in single_cmd_execute()"DEFAULT);
				gc_free(gc);
				exit(1);
			}
		}
		execute_builtin(cur, shell);
	}
	else if (cur->is_external_cmd)
	{
		pid = fork();
		shell->pids[0] = pid;
		fprintf(stderr, YELLOW" singlecmd for child proc fork() : %d , shell->pids[0] %d \n"DEFAULT, pid, shell->pids[0]) ;
		if (pid == 0) //signal implementation?
		{
		
			if (cur->io_streams && cur->io_streams->heredoc_eof)
			{
				if (heredoc_fd_offset_and_redir(cur) == -1)
				{
					perror(RED"heredoc error in single_cmd_execute()"DEFAULT);
					gc_free(gc);
					exit(1);
				}
			}
			run_execve(cur, gc);
		}
		else
			wait_for_child_and_update_status(1);
	}
}

void	execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	if (cur->io_streams && cur->io_streams->heredoc_eof)
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
	if (cur->built_in)
	{
		fprintf(stderr, RED"is it in pipe for execute_builtin execute_child()\n"DEFAULT);
		execute_builtin(cur, shell);
	}
	if (cur->args)
	{
		fprintf(stderr, RED"is it in pipe for external cmd execute_child()\n"DEFAULT);
		run_execve(cur, gc);
	}
}

void 	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	char	**splitted_path;
	t_shell	*shell;

	shell = get_shell();
	char *path = find_var_in_env(shell->my_envp, "PATH", 4, gc->temp);
	if (!path)
	{
		perror(RED"can't find PATH"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	if (cmd_block->args[0][0] == '/' || ft_strncmp(cmd_block->args[0], "./", 2) == 0)
	{
		if (access(cmd_block->args[0], F_OK | X_OK) == 0)
		{
			signal(SIGINT, signal_handler_for_child);
			signal(SIGQUIT, signal_handler_for_child);
			execve(cmd_block->args[0], cmd_block->args, shell->my_envp);
			perror(RED"error execve() (absolute path)"DEFAULT);
			gc_free(gc);
			exit(1);
		}
		fprintf(stderr, RED"command not found (absolute path): %s\n"DEFAULT, cmd_block->args[0]);
		exit(127);
	}
	else
	{
		splitted_path = ft_split(path, ':');
	free(path);
	if (!splitted_path)
	{
		perror(RED"ft_split failed in run_execve()"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	int i = 0;
	while(splitted_path[i])
	{
		char *attach_slash_to_cmd = gc_strjoin(splitted_path[i], "/", &gc->temp);
		if (!attach_slash_to_cmd)
		{
			gc_free(gc);
			exit(1);
		}
		char *cmd_path = gc_strjoin(attach_slash_to_cmd, cmd_block->args[0], &gc->temp);
		if (!cmd_path)
		{
			gc_free(gc);
			exit(1);
		}
		fprintf(stderr, YELLOW"cmd_path: %s\n"DEFAULT, cmd_path);
		fprintf(stderr, YELLOW"cmd_block->args[0]: %s\n"DEFAULT, cmd_block->args[0]);
		fprintf(stderr, YELLOW"cmd_block->args[1]: %s\n"DEFAULT, cmd_block->args[1]);	
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			signal(SIGINT, signal_handler_for_child);
			signal(SIGQUIT, signal_handler_for_child);
			fprintf(stderr, "execve()\n");
			if (execve(cmd_path , cmd_block->args, shell->my_envp) == -1)
			{
				gc_free(gc);
				perror(RED"error execve()"DEFAULT);
				exit(1);
			}
		}
		i++;
	}
	fprintf(stderr, RED"command not found: %s\n"DEFAULT, cmd_block->args[0]);
	exit(127);
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
		fprintf(stderr, YELLOW"[pid %d] p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT,getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
	}
	pid = fork();
	fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
	if (pid == 0) //signal function call?
		execute_child(cur, gc, shell);
	close_pipefd(cur);
	shell->pids[*i] = pid;
}
