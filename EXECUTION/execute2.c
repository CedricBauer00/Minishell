/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:37:15 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/12 12:46:22 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*check_path_before_exec(t_shell *shell, t_gc *gc)
{
	char *path;
	path = find_var_in_env(shell->my_envp, "PATH", 4);
	if (!path)
	{
		printf(RED"can't find PATH\n"DEFAULT);
		gc_free(gc);
		exit(127);
	}
	return path;
}

static void execute(char *arg, char **args, t_shell *shell)
{
	if (access(arg, F_OK | X_OK) == 0)
	{
		if (execve(arg, args, shell->my_envp) == -1)
		{
			perror(RED"execve ()failed"DEFAULT);
			exit(1);
		}
	}
}

static void foo(char *path, t_cmd_block *cmd_block, t_gc *gc, t_shell *shell)
{
	int	i;
	char	**splitted_path;
	char	*cmd_path;
	char	*attach_slash_to_cmd;

	i = 0;
	splitted_path = ft_split(path, ':');
	free(path);
	is_exited(splitted_path, gc);
	while (splitted_path[i])
	{
		attach_slash_to_cmd = gc_strjoin(splitted_path[i], "/", &gc->temp);
		is_exited(attach_slash_to_cmd, gc);
		cmd_path = gc_strjoin(attach_slash_to_cmd, cmd_block->args[0], &gc->temp);
		is_exited(cmd_path, gc);
		execute(cmd_path, cmd_block->args, shell);
		i++;
	}
	perror("command not found");
    exit(127);
}

void	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	t_shell	*shell;
	char	*path;

	if (!cmd_block || !cmd_block->args || !cmd_block->args[0])
		return ;
	shell = get_shell();
	path = check_path_before_exec(shell, gc);
	if (cmd_block->args[0][0] == '/' || ft_strncmp(cmd_block->args[0], "./", 2) == 0)
	{
		execute(cmd_block->args[0], cmd_block->args, shell);
	}
	else
	{
		foo(path, cmd_block, gc, shell);
	}
	perror(RED"no such file"DEFAULT);
	exit(127);
}

static void 	execute_single_cmd(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	pid_t	pid;

	pid = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	shell->pids[0] = pid;
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		run_execve(cur, gc);
	}
	else
		wait_for_child_and_update_status(1);
}

void	single_cmd_execute(t_cmd_block *cur, t_gc *gc)
{
	t_shell	*shell;

	if (!cur)
		return ;
	shell = get_shell();
	if (cur->io_streams && cur->io_streams->heredoc_eof)
	{
		if (heredoc_fd_offset_and_redir(cur) == -1)
		{
			gc_free(gc);
			exit(1);
		}
	}
	if (cur->io_streams)
		set_io_streams(cur);
	if (cur->is_built_in)
		execute_builtin(cur, shell);
	else if (cur->is_external_cmd)
	{
		execute_single_cmd(cur, gc, shell);
	}
}

int	heredoc_fd_offset_and_redir(t_cmd_block *cur)
{
	t_shell	*shell;

	shell = get_shell();
	if (!cur)
		return (-1);
	shell->heredoc_fd = open("temp_heredoc", O_RDWR | O_CREAT, 0644);
	if (shell->heredoc_fd < 0)
	{
		return (-1);
	}
	if (dup2(shell->heredoc_fd, STDIN_FILENO) == -1)
	{
		fprintf(stderr, "in heredoc_fd_offset_and_redir 2 \n");
		close(shell->heredoc_fd);
		unlink("temp_heredoc");
		return (-1);
	}
	close(shell->heredoc_fd);
	unlink("temp_heredoc");
	return (1);
}

void	execute_builtin(t_cmd_block *cur, t_shell *shell)
{
	t_gc	*gc;

	gc = get_gc();
	if (ft_strcmp(cur->built_in, "cd") == 0)
	{
		fprintf(stderr, "in execute_builtin()\n");
		cd(cur->args, shell, gc);
	}
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
		wait4(shell->pids[idx], &status, 0, NULL);
		if (WIFEXITED(status) && WEXITSTATUS(status))
			shell->last_status_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->last_status_exit = 128 + WTERMSIG(status);
			return ;
		}
		idx++;
	}
}
