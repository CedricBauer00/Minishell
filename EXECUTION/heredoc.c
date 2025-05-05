/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:15 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/05 16:57:43 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//fd sollte einfach STDIN_FILENO
// char *get_terminal_path(int fd)
// {
// 	char *tty_path;
	
// 	if (fd < 0)
// 		return (NULL);
// 	tty_path = ttyname(fd);
// 	if (!tty_path)
// 		return (NULL);
// 	return (tty_path);
// }

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
	printf("signal!%d\n", shell->heredoc_interrupted);
	while(1)
	{
		char *line;
		line = readline("> ");
		if (!line)
			break;
		size_t len = ft_strlen(line);
		char *temp = do_alloc(&gc->temp, len + 1, TYPE_SINGLE_PTR, "heredoc");
		ft_strlcpy(temp, line, len + 1);
		free(line);
		if (!temp || strcmp(temp, token->value) == 0)
		{
			break;
		}
		write(fd_heredoc, temp, strlen(temp));
		write(fd_heredoc, "\n", 1);
	}
	close(fd_heredoc);
	gc_free(gc);
	fprintf(stderr, YELLOW"[pid %d] close() %d\n"DEFAULT,getpid(), fd_heredoc);
	return 1;
}


//bei der durchfuehrung des cmd muss ich den FD des temporäre Datei umleiten.
// int main()
// {
// 	heredoc();
// }