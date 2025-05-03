/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:11:32 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/03 17:11:33 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// # include <readline/readline.h>
// # include <readline/history.h>

/* printf함수를 사용하기위한 헤더 */
// # include <stdio.h>

// /* free함수를 사용하기위한 헤더 */
// # include <stdlib.h>

/*

*/
//termianl path 


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


int	process_heredoc(t_io_streams_list *io_streams)
{
	int	fd_heredoc;
	
	fd_heredoc = 0;
	while (io_streams && io_streams->heredoc_eof)
	{
		fd_heredoc = open("temp_heredoc", O_RDWR | O_CREAT | O_TRUNC, 0644);
		fprintf(stderr, YELLOW"[pid %d] fd_heredoc open(), fd_heredoc fd : %d\n", getpid(), fd_heredoc);
		if (fd_heredoc == -1)
		{
			perror(RED"failed to open temp_heredoc"DEFAULT);
			return -1;
		}
		while(1)
		{
			char *line;
			line = readline("> ");
			if (!line || strcmp(line, io_streams->heredoc_eof) == 0)
			{
				free(line);
				break;
			}
			write(fd_heredoc, line, strlen(line));
			write(fd_heredoc, "\n", 1);
			free(line);
		}
		io_streams->heredoc_fd = fd_heredoc;
		close(fd_heredoc);
		fprintf(stderr, YELLOW"[pid %d] close()%d\n"DEFAULT,getpid(), fd_heredoc);
		io_streams = io_streams->next;
	}
	return 1;
}


//bei der durchfuehrung des cmd muss ich den FD des temporäre Datei umleiten.
// int main()
// {
// 	heredoc();
// }