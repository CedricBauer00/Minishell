/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/09 17:49:46 by jisokim2         ###   ########.fr       */
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
			// printf("case1\n");
			i++;
			while(line[i] && (ft_isalpha(line[i]) || line[i] == '_'))
			{
				i++;
			}
			key =  ft_substr(line, start, i - start);
			// fprintf(stderr, "key : %s\n", key);
			k = check_existing(shell->my_envp, &key[1]);
			// fprintf(stderr, "k %d\n", k);
			if (k >= 0)
			{
				value = extract_value(shell->my_envp[k]);
				// fprintf(stderr, "value %s\n", value);
			}
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
	// fd_heredoc = open("temp_heredoc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	// fprintf(stderr, YELLOW"[pid %d] fd_heredoc open(), fd_heredoc fd : %d\n", getpid(), fd_heredoc);
	// shell->heredoc_fd = fd_heredoc;
	// if (fd_heredoc == -1)
	// {
	// 	perror(RED"failed to open temp_heredoc"DEFAULT);
	// 	gc_free(gc);
	// 	exit(1);
	// }
	signal(SIGINT, SIG_DFL);
	while(1)
	{
		char *line;
		line = readline("> ");
		if (!line)
		{
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
			// fprintf(stderr, YELLOW"[pid %d] close() %d\n"DEFAULT,getpid(), fd_heredoc);
			exit(0);
			break;
		}
		write(fd_heredoc, expanded_var, strlen(expanded_var));
		write(fd_heredoc, "\n", 1);
		free(line);
	}
}

int	wait_for_heredoc_pid(pid_t heredoc_pid, int status)
{
	t_shell *shell;

	shell = get_shell();
	signal(SIGINT, SIG_IGN);
	waitpid(heredoc_pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		// printf(BLUE"1\n"DEFAULT);
		shell->last_status_exit = 1;
		return (-1);
	}
	else if (WIFEXITED(status))
	{
		int exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
		{
			// printf(BLUE"2\n"DEFAULT);
			shell->last_status_exit = exit_status;
			return (1);
		}
	}
	return 0;
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
	int status = 0;
	pid_t pid;
	pid = 0;
	int stdin_backup;
	int stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
    stdout_backup = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
		process_heredoc(shell, cur);
	else if (pid > 0)
	{
		int test = wait_for_heredoc_pid(pid, status);
		//printf("test%d\n", test);
		if (test == -1)
			return -1;
		dup2(stdin_backup, STDIN_FILENO);
    	dup2(stdout_backup, STDOUT_FILENO);
    	close(stdin_backup);
    	close(stdout_backup);
		if (ttyattr() < 0)
			return (printf("ERROR\nttyattr failed!\n"), -1);
	}
	return 0;
}

