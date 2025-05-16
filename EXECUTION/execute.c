/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/16 14:24:53 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_builtin(t_cmd_block *cur, t_shell *shell)
{
	t_gc	*gc;

	if (!shell || !cur->args)
		return ;
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
		ft_exit(cur->args, shell, gc);
	return ;
}

void	main_execute(t_cmd_block *cmd_block)
{
	int			pid_counts;

	if (!cmd_block)
		return ;
	t_shell		*shell;
	shell = get_shell();
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	pid_counts = count_command(cmd_block);
	do_alloc_pids(cmd_block);
	if (pid_counts == 1)
	{
		execute_single_command(cmd_block);
	}
	if (pid_counts > 1)
	{
		execute_pipeline(cmd_block);
	}
	prevent_zombie_process();
	dup2(shell->stdin_backup, STDIN_FILENO);
	dup2(shell->stdout_backup, STDOUT_FILENO);
	close(shell->stdin_backup);
	close(shell->stdout_backup);
}

void	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	t_shell	*shell;
	char	*path;

	if (!cmd_block || !cmd_block->args || !cmd_block->args[0])
		return ;
	shell = get_shell();
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd_block->args[0][0] == '/'
		|| ft_strncmp(cmd_block->args[0], "./", 2) == 0)
	{
		access_and_exec(cmd_block->args[0], cmd_block->args, shell);
	}
	else
	{
		path = check_path_before_exec(shell, gc);
		exec_relative_path(path, cmd_block, gc, shell);
	}
	printf("minishell: %s : %s\n", cmd_block->args[0], strerror(errno));
	exit(127);
}

void	exec_relative_path(char *path, t_cmd_block *cmd_block, \
	t_gc *gc, t_shell *shell)
{
	int		i;
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
		cmd_path = gc_strjoin(attach_slash_to_cmd, \
			cmd_block->args[0], &gc->temp);
		is_exited(cmd_path, gc);
		access_and_exec(cmd_path, cmd_block->args, shell);
		i++;
	}
	printf("minishell: %s: command not found\n", cmd_block->args[0]);
	exit(127);
}

void	execute_pipeline(t_cmd_block *cmd_block)
{
	t_gc		*gc;
	int			i;

	i = 0;
	gc = get_gc();
	if (!cmd_block)
		return ;
	while (cmd_block)
	{
		if (cmd_block->next)
		{
			add_pipe(&cmd_block);
			fprintf(stderr, "[pid %d] pipe ( %d, %d ) \n",getpid(), cmd_block->pipe->pipefd[0], cmd_block->pipe->pipefd[1]);
		}
		if(cmd_block->is_built_in || cmd_block->is_external_cmd || cmd_block->io_streams->heredoc_eof)
		{
			fork_and_execute(cmd_block, gc, &i);
			i++;
		}
		cmd_block = cmd_block->next;
	}
	wait_for_child_and_update_status(i);
}
