/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/11 12:49:57 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*expand_case_in_heredoc(char *line ,t_shell *shell)
{
	t_gc	*gc;
	char	*value;
	int		i;
	int		k;
	char	*key;
	
	k = 0;
	gc = get_gc();
	char *result = gc_strdup("", &gc->temp);
	value = NULL;
	i = 0;
	while(line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			int start = i;
			i++;
			while(line[i] && (ft_isalpha(line[i]) || line[i] == '_'))
				i++;
			key =  ft_substr(line, start, i - start);
			k = check_existing(shell->my_envp, &key[1]);
			if (k >= 0)
				value = extract_value(shell->my_envp[k]);
			else
			{
				result = gc_strdup("", &gc->temp);
				return result;
			}
			result = gc_strjoin(result, value, &gc->temp);
			free(key);
		}
		else
		{
			char str[2];
			str[0] = line[i];
			str[1] = '\0';
			result = gc_strjoin(result, str, &gc->temp);
			i++;
		}
	}
	return (result);
}

void	process_heredoc(t_shell *shell, t_token *token)
{
	int	fd_heredoc = shell->heredoc_fd;
	t_gc *gc;

	gc = get_gc();
	while(1)
	{
		char *line;
		line = readline("> ");
		if (!line)
		{
			fprintf(stderr, "ctrl  + d in heredoc \n");
			close(fd_heredoc);
			gc_free(gc);
			exit(0);
		}
		char *expanded_var = expand_case_in_heredoc(line, shell);
		size_t len = ft_strlen(line);
		char *temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
		ft_strlcpy(temp, expanded_var, len + 1);
		if (!line || strcmp(line, token->value) == 0)
		{
			free(line);
			close(fd_heredoc);
			gc_free(gc);
			exit(0);
			break;
		}
		write(fd_heredoc, expanded_var, strlen(expanded_var));
		write(fd_heredoc, "\n", 1);
		free(line);
	}
}

void	wait_for_heredoc_pid(pid_t heredoc_pid, int status)
{
	t_shell	*shell;

	shell = get_shell();
	
	waitpid(heredoc_pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		shell->last_status_exit = 1;
		return ;
	}
	else if (WIFEXITED(status))
	{
		int exit_status = WEXITSTATUS(status);
		shell->last_status_exit = exit_status;
		return ;
	}
	return ;
}
static void heredoc_sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    exit(1);
}

void	execute_heredoc(t_shell *shell, t_token *cur)
{
	int		status;
	pid_t	pid;
	int		stdin_backup;
	int		stdout_backup;
	int		test;

	pid = 0;
	status = 0;
	stdin_backup = dup(STDIN_FILENO);
    stdout_backup = dup(STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_DFL);	
		process_heredoc(shell, cur);
	}
	else if (pid > 0)
	{
		wait_for_heredoc_pid(pid, status);
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
	}
}
