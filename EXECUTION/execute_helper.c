/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/13 10:26:29 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	do_alloc_pids(t_cmd_block *cmd_block)
{
	int		count;
	t_shell	*shell;
	t_gc	*gc;

	gc = get_gc();
	count = count_command(cmd_block);
	if (count == 0 || (count == 1 && cmd_block->built_in))
		return ;
	shell = get_shell();
	shell->pids = do_alloc(&gc->temp, sizeof(pid_t) * count, \
		TYPE_SINGLE_PTR, "pids");
	if (!shell->pids)
	{
		gc_free(gc);
		exit(1);
	}
}

int	count_command(t_cmd_block *cmd_block)
{
	t_cmd_block	*temp;
	int			count;

	temp = cmd_block;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
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
		printf("in heredoc_fd_offset_and_redir 2 \n"); // do we need?
		close(shell->heredoc_fd);
		unlink("temp_heredoc");
		return (-1);
	}
	close(shell->heredoc_fd);
	unlink("temp_heredoc");
	return (1);
}

char	*check_path_before_exec(t_shell *shell, t_gc *gc)
{
	char	*path;

	path = find_var_in_env(shell->my_envp, "PATH", 4);
	if (!path)
	{
		printf("can't find PATH\n");
		gc_free(gc);
		exit(127);
	}
	return (path);
}

void	access_and_exec(char *arg, char **args, t_shell *shell)
{
	if (access(arg, F_OK | X_OK) == 0)
	{
		if (execve(arg, args, shell->my_envp) == -1)
		{
			perror("execve ()failed");// do we need? correct message?
			exit(1);
		}
	}
}
