/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/06 12:50:12 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	process_heredoc(t_shell *shell, t_token *token)
{
	int	fd_heredoc;
	t_gc *gc;

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
		size_t len = ft_strlen(line);
		char *temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
		ft_strlcpy(temp, line, len + 1);

		char *expand = ft_strchr(temp, '$');
		//tmep == $USER;
		int	i = 0;
		if (expand && expand[i] == '$' && expand[i + 1] != '\0')
		{
			int j = check_existing(shell->my_envp, &expand[i + 1]);
			if (j > 0)
				fprintf(stderr,YELLOW"expand case in heredoc : %s\n"DEFAULT, shell->my_envp[j]);
			char *result = extract_value(shell->my_envp[j]);
			fprintf(stderr,YELLOW"expand case in heredoc : %s\n"DEFAULT, result);
			// char *temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
			// ft_strlcpy(temp, line, len + 1);
			// free(line);
			write(fd_heredoc, result, strlen(result));
			write(fd_heredoc, "\n", 1);
		}
		
		free(line);
		if (!temp || strcmp(temp, token->value) == 0)
		{
			gc_free(gc);
			exit(0);
			break;
		}
		write(fd_heredoc, temp, strlen(temp));
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