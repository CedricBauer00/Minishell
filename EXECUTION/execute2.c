/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:37:15 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/10 13:49:56 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
		if (pid == 0)
		{
			if (cur->io_streams && cur->io_streams->heredoc_eof)
			{
				if (heredoc_fd_offset_and_redir(cur) == -1)
				{
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

int heredoc_fd_offset_and_redir(t_cmd_block *cur)
{
	t_shell	*shell;

	shell = get_shell();
	if (!cur)
		return -1;
	shell->heredoc_fd = open("temp_heredoc" ,O_RDWR | O_CREAT, 0644);
	if (shell->heredoc_fd < 0)
	{
		return -1;
	}
	if (dup2(shell->heredoc_fd, STDIN_FILENO) == -1)
	{
		fprintf(stderr, "in heredoc_fd_offset_and_redir 2 \n");
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
	if (ft_strcmp(cur->built_in, "cd") == 0)
		cd(cur->args, shell, gc);
	else if (ft_strcmp(cur->built_in, "echo") == 0)
		ft_echo(cur->args, true, 0, 1);
	else if (ft_strcmp(cur->built_in, "export") == 0)
		export(cur->args, shell);
	else if (ft_strcmp(cur->built_in, "pwd") == 0)
		ft_pwd(cur->args, gc);
	else if (ft_strcmp(cur->built_in, "env") == 0)
		ft_env(cur->args, shell);
	else if (ft_strcmp(cur->built_in, "unset") == 0)
		ft_unset(cur->args, shell);
	else if (ft_strcmp(cur->built_in, "exit") == 0)
		ft_exit(cur->args, shell);
	return ;
}

void 	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	char	**splitted_path;
	t_shell	*shell;

	if ( !cmd_block)
		return ;
	shell = get_shell();
	char *path = find_var_in_env(shell->my_envp, "PATH", 4);
	if (!path)
	{
		printf(RED"can't find PATH"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	if (!cmd_block->args || !cmd_block->args[0])
		return ;
	fprintf(stderr, "cmd_block->args[0]%s\n", cmd_block->args[0]);
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
		printf("2\n");
		splitted_path = ft_split(path, ':');
		free(path);
		if (!splitted_path)
		{
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
			if (access(cmd_path, F_OK | X_OK) == 0)
			{
				signal(SIGINT, signal_handler_for_child);
				signal(SIGQUIT, signal_handler_for_child);
				if (execve(cmd_path , cmd_block->args, shell->my_envp) == -1)
				{
					perror(RED"error execve()"DEFAULT);
					exit(1);
				}
			}
			i++;
		}
		printf(RED"command not found\n"DEFAULT);
	
		exit(127);
	}
	return ;
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
		wait4(shell->pids[idx], &status, 0 ,NULL);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			shell->last_status_exit = WEXITSTATUS(status);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			shell->last_status_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_status_exit = 128 + WTERMSIG(status);
		idx++;
	}
}
