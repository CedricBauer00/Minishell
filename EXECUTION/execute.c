/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/13 15:55:19 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_builtin(t_cmd_block *cur, t_shell *shell)
{
	t_gc	*gc;

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
	t_cmd_block	*cur;
	int			stdin_backup;
	int			stdout_backup;
	int			pid_counts;

	cur = cmd_block;
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	pid_counts = count_command(cmd_block);
	do_alloc_pids(cmd_block);
	if (pid_counts == 1)
		execute_single_command(cur);
	if (pid_counts > 1)
		execute_pipeline(cur);
	prevent_zombie_process();
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

void	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	t_shell	*shell;
	char	*path;

	if (!cmd_block || !cmd_block->args || !cmd_block->args[0])
		return ;
	shell = get_shell();
	path = check_path_before_exec(shell, gc);
	if (cmd_block->args[0][0] == '/' ||
			ft_strncmp(cmd_block->args[0], "./", 2) == 0)
		access_and_exec(cmd_block->args[0], cmd_block->args, shell);
	else
		exec_relative_path(path, cmd_block, gc, shell);
	printf("No such file or DIR"); //DIR uppercase?
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
	printf("minishell: : command not found\n"); //minishell: "input": command not found
	exit(127);
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
		if (cur->is_built_in || cur->is_external_cmd)
			fork_and_execute(cur, gc, &i);
		i++;
		cur = cur->next;
	}
	wait_for_child_and_update_status(i);
}
