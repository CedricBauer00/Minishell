/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/06 17:04:39 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// static int	ft_isspace(char c)
// {
// 	return (c == ' ' || c == '\t' || c == '\n'
// 		|| c == '\r' || c == '\f' || c == '\v');
// }

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
			//int start = i;
			printf("case1\n");
			i++;
			while(line[i] && (line[i] || line[i] == '_'))
			{
				i++;	
			}
			key = ft_strchr(line, '$');
			// ft_substr()
			fprintf(stderr, "key : %s\n", key);
			k = check_existing(shell->my_envp, &key[1]);
			fprintf(stderr, "k %d\n", k);
			if (k > 0)
			{
				value = extract_value(shell->my_envp[k]);
				fprintf(stderr, "value %s\n", value);
			}
			else
			{
				printf("case2\n");
				value = "";
				i++;
			}
			result = gc_strjoin(result, value, &gc->temp);
			fprintf(stderr, RED"result : %s\n"DEFAULT, result);
		}
		else
		{
			printf("case3\n");
			result = "";
			result = gc_strjoin(result, line, &gc->temp);
			fprintf(stderr, BLUE"result : %s\n"DEFAULT, result);
			i++;
		}
	}
	return (result);
}

int	process_heredoc(t_shell *shell, t_token *token)
{
	int	fd_heredoc;
	t_gc *gc;
	// t_main	*main;
	
	// main = get_main();
	gc = get_gc();
	fd_heredoc = 0;
	fd_heredoc = open("temp_heredoc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	fprintf(stderr, YELLOW"[pid %d] fd_heredoc open(), fd_heredoc fd : %d\n", getpid(), fd_heredoc);
	shell->heredoc_fd = fd_heredoc;
	if (fd_heredoc == -1)
	{
		perror(RED"failed to open temp_heredoc"DEFAULT);
		return -1;
	}
	signal(SIGINT, SIG_DFL);
	while(1)
	{
		char *line;
		line = readline("> ");
		if (!line)
			break;
		char *expanded_var = expand_case_in_heredoc(line, shell);
		
		size_t len = ft_strlen(line);
		char *temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
		ft_strlcpy(temp, expanded_var, len + 1);
		free(line);
		if (!temp || strcmp(temp, token->value) == 0)
		{
			gc_free(gc);
			exit(0);
			break;
		}
		write(fd_heredoc, expanded_var, strlen(expanded_var));
		write(fd_heredoc, "\n", 1);
	}
	close(fd_heredoc);
	gc_free(gc);
	fprintf(stderr, YELLOW"[pid %d] close() %d\n"DEFAULT,getpid(), fd_heredoc);
	exit(0);
	return 1;
}


//bei der durchfuehrung des cmd muss ich den FD des temporäre Datei umleiten.
// int main()
// {
// 	heredoc();
// }