/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/12 12:46:31 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char *extract_expanded_env(char *line, int *i, t_shell *shell, t_gc *gc)
{
	char	*value;
	char	*key;
	int		idx;
	int		start;

	start = 0;
	idx = -1;
	value = NULL;
	start = *i;
	(*i)++;
	while(line[*i] && (ft_isalpha(line[*i]) || line[*i] == '_'))
		(*i)++;
	key =  gc_substr(line, start, *i - start, gc);
	idx = check_existing(shell->my_envp, &key[1]);
	if (idx >= 0)
		value = extract_value(shell->my_envp[idx]);
	else
		value = gc_strdup("", &gc->temp);
	return value;
}

static char *non_expanded_case(char *line, char *result, int *i, t_gc *gc)
{
	char str[2];

	str[0] = line[*i];
	str[1] = '\0';
	result = gc_strjoin(result, str, &gc->temp);
	(*i)++;
	return result;
}

static char	*expand_case_in_heredoc(char *line ,t_shell *shell)
{
	t_gc	*gc;
	char	*expanded;
	int		i;
	char	*result;
	
	gc = get_gc();
	result = gc_strdup("", &gc->temp);
	expanded = NULL;
	i = 0;
	while(line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			expanded = extract_expanded_env(line ,&i, shell ,gc);
			result = gc_strjoin(result, expanded, &gc->temp);
		}
		else
		{
			result = non_expanded_case(line, result, &i, gc);
		}
	}
	return (result);
}

//memo it is too complicated so maybe i can change lie this.
// void	process_heredoc(t_shell *shell, t_token *token)
// {
// 	int	fd_heredoc = shell->heredoc_fd;
// 	t_gc *gc;

// 	gc = get_gc();
// 	while(1)
// 	{
// 		char *line;
// 		line = readline("heredoc > ");
// 		if (!line)
// 		{
// 			//fprintf(stderr, "ctrl  + d in heredoc \n");
// 			close(fd_heredoc);
// 			gc_free(gc);
// 			exit(0);
// 		}

	
// 		char *expanded_var = expand_case_in_heredoc(line, shell);
// 		size_t len = ft_strlen(line);
// 		char *temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
// 		ft_strlcpy(temp, expanded_var, len + 1);
// 		if (!line || strcmp(line, token->value) == 0)
// 		{
// 			free(line);
// 			close(fd_heredoc);
// 			gc_free(gc);
// 			exit(0);
// 			break;
// 		}
// 		write(fd_heredoc, expanded_var, strlen(expanded_var));
// 		write(fd_heredoc, "\n", 1);
// 		free(line);
// 	}
// }

void	process_heredoc(t_shell *shell, t_token *token)
{
	t_gc *gc;

	gc = get_gc();
	while(1)
	{
		char *line;
		line = readline("> ");
		if (!line || strcmp(line, token->value) == 0)
		{
			free(line);
			close(shell->heredoc_fd);
			close(shell->stdin_backup);
			close(shell->stdout_backup);
			gc_free(gc);
			exit(0);
		}
		char *expanded_var = expand_case_in_heredoc(line, shell);
		write(shell->heredoc_fd, expanded_var, strlen(expanded_var));
		write(shell->heredoc_fd, "\n", 1);
		free(line);
	}
}

int	wait_for_heredoc_pid(pid_t heredoc_pid, int status)
{
	t_shell	*shell;

	shell = get_shell();
	waitpid(heredoc_pid, &status, 0);
	//memo never go into this if statment
	// if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	// {
	// 	int exit_status = WTERMSIG(status);
    //     shell->last_status_exit = 128 + exit_status;
	// 	fprintf(stderr, "1\n");
	// 	close(shell->heredoc_fd);
	// 	close(shell->stdin_backup);
	// 	close(shell->stdout_backup);
	// 	unlink("temp_heredoc");
	// 	return shell->last_status_exit;
	// }
	if (WIFEXITED(status))
	{
		int exit_status = WEXITSTATUS(status);
		shell->last_status_exit = exit_status;
		close(shell->heredoc_fd);
		close(shell->stdin_backup);
		close(shell->stdout_backup);
		return shell->last_status_exit;
	}
	return 0;
}

static void heredoc_sigint_handler(int sig)
{
	t_shell *shell;
	t_gc *gc;

	gc = get_gc();
	shell = get_shell();
	if (sig == SIGINT)
	{
		close(shell->heredoc_fd);
		close(shell->stdin_backup);
		close(shell->stdout_backup);
		if (gc)
		 	gc_free(gc);
	}
    write(1, "\n", 1);
    exit(1);
}

int	execute_heredoc(t_shell *shell, t_token *cur)
{
	int		status;
	pid_t	pid;

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
		int test = wait_for_heredoc_pid(pid, status);
		if (test == 1)
			return 1;
		dup2(shell->stdin_backup, STDIN_FILENO);
		dup2(shell->stdout_backup, STDOUT_FILENO);
	}
	return 0;
}
