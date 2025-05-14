/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/14 15:18:10 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	clean_heredoc(t_shell *shell, bool remove_file)
{
	if (shell)
	{
		close(shell->heredoc_fd);
		close(shell->stdin_backup);
		close(shell->stdout_backup);
	}
	if (remove_file)
		unlink("temp_heredoc");
}
static int	wait_for_heredoc_pid(pid_t heredoc_pid, int status)
{
	t_shell	*shell;
	int		exit_status;

	shell = get_shell();
	waitpid(heredoc_pid, &status, 0);
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		shell->last_status_exit = exit_status;
		clean_heredoc(shell, false);
	}
	return (shell->last_status_exit);
}

void	heredoc_sigint_handler(int sig)
{
	t_shell	*shell;
	t_gc	*gc;

	gc = get_gc();
	shell = get_shell();
	if (sig == SIGINT)
	{
		clean_heredoc(shell, true);
		write(1, "\n", 1);
		exit(1);
	}
}

void	process_heredoc(t_shell *shell, t_token *token)
{
	t_gc	*gc;
	char	*line;
	char	*expanded_var;

	gc = get_gc();
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, token->value) == 0)
		{
			free(line);
			clean_heredoc(shell, false);
			//gc_free(gc);
			exit(0);
		}
		expanded_var = expand_case_in_heredoc(line, shell);
		write(shell->heredoc_fd, expanded_var, ft_strlen(expanded_var));
		write(shell->heredoc_fd, "\n", 1);
		free(line);
	}
}

int	execute_heredoc(t_shell *shell, t_token *cur)
{
	int		status;
	pid_t	pid;
	int		test;

	pid = 0;
	status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		process_heredoc(shell, cur);
	}
	else if (pid > 0)
	{
		test = wait_for_heredoc_pid(pid, status);
		if (test == 1)
			return (1);
		else
			fprintf(stderr, "test : %d\n", test);
		dup2(shell->stdin_backup, STDIN_FILENO);
		dup2(shell->stdout_backup, STDOUT_FILENO);
	}
	fprintf(stderr, "suc\n");
	return (0);
}

