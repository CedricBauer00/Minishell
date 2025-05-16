/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/16 14:18:43 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_single_command(t_cmd_block *cmd_block)
{
	t_gc	*gc;

	gc = get_gc();
	if (cmd_block && !cmd_block->prev && !cmd_block->next)
		single_cmd_execute(cmd_block, gc);
}

void	single_cmd_execute(t_cmd_block *cur, t_gc *gc)
{
	t_shell	*shell;

	if (!cur)
		return ;
	shell = get_shell();
	if (cur->io_streams && cur->io_streams->heredoc_eof)
	{
		heredoc_fd_offset_and_redir(cur);
	}
	if (cur->io_streams)
		if (set_io_streams(cur) == -1)
			return ;
	if (cur->is_built_in)
		execute_builtin(cur, shell);
	else if (!cur->is_built_in || cur->args)
	{
		execute_single_external_cmd(cur, gc, shell);
	}
}

void	execute_single_external_cmd(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	pid_t	pid;

	if (!cur)
		return ;
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
	else if (pid > 0)
	{
		wait_for_child_and_update_status(1);
	}
}
