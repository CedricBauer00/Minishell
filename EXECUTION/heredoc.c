/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 12:16:13 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*expand_case_in_heredoc(char *line, t_shell *shell)
{
	t_gc	*gc;
	char	*value;
	int		i;
	int		k;
	char	*key;
	char	*result;
	int		start;
	char	str[2];

	i = 0;
	k = 0;
	value = NULL;
	gc = get_gc();
	result = gc_strdup("", &gc->temp);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			start = i;
			i++;
			while (line[i] && (ft_isalpha(line[i]) || line[i] == '_'))
				i++;
			key = ft_substr(line, start, i - start);
			k = check_existing(shell->my_envp, &key[1]);
			if (k >= 0)
				value = extract_value(shell->my_envp[k]);
			else
			{
				result = gc_strdup("", &gc->temp);
				return (result);
			}
			result = gc_strjoin(result, value, &gc->temp);
			free(key);
		}
		else
		{
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
	int		fd_heredoc;
	t_gc	*gc;
	size_t	len;
	char	*temp;
	char	*line;
	char	*expanded_var;

	fd_heredoc = shell->heredoc_fd;
	gc = get_gc();
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd_heredoc);
			gc_free(gc);
			exit(0);
		}
		expanded_var = expand_case_in_heredoc(line, shell);
		len = ft_strlen(line);
		temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
		ft_strlcpy(temp, expanded_var, len + 1);
		if (!line || strcmp(line, token->value) == 0)
		{
			free(line);
			close(fd_heredoc);
			gc_free(gc);
			exit(0);
			break ;
		}
		write(fd_heredoc, expanded_var, strlen(expanded_var));
		write(fd_heredoc, "\n", 1);
		free(line);
	}
}

int	wait_for_heredoc_pid(pid_t heredoc_pid, int status)
{
	t_shell	*shell;
	int		exit_status;

	shell = get_shell();
	signal(SIGINT, SIG_IGN);
	waitpid(heredoc_pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		shell->last_status_exit = 1;
		return (-1);
	}
	else if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
		{
			shell->last_status_exit = exit_status;
			return (1);
		}
	}
	return (0);
}

void	signal_func(int sig)
{
	t_shell	*shell;

	shell = get_shell();
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	shell->last_status_exit = 1;
}

int	ttyattr(void)
{
	struct termios	temp;

	if (tcgetattr(0, &temp) < 0)
		return (-1);
	temp.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(0, TCSANOW, &temp) < 0)
		return (-1);
	return (0);
}

int	execute_heredoc(t_shell *shell, t_token *cur)
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
	pid = fork();
	if (pid == 0)
		process_heredoc(shell, cur);
	else if (pid > 0)
	{
		test = wait_for_heredoc_pid(pid, status);
		if (test == -1)
			return (-1);
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		if (ttyattr() < 0)
			return (printf("ERROR\nttyattr failed!\n"), -1);
	}
	return (0);
}
