/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:26 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/12 15:55:57 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	error_redir_open(void)
{
	t_gc	*gc;

	gc = get_gc();
	gc_free(gc);
	exit(1);
}

static void	open_redir_file(int *fd, char *filename, int flags, mode_t mode)
{
	*fd = open(filename, flags, mode);
	if (*fd == -1)
	{
		perror(RED"failed open for redir"DEFAULT);
		error_redir_open();
	}
}

static void	dup2_redir(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror(RED"failed open for redir"DEFAULT);
		error_redir_open();
	}
}

void	handle_redir(t_io_streams_list	*io_streams)
{
	int	fd;

	fd = -1;
	if (io_streams->infile_name)
	{
		open_redir_file(&fd, io_streams->infile_name, O_RDONLY, 0);
		dup2_redir(fd, STDIN_FILENO);
		close(fd);
	}
	if (io_streams->outfile_name)
	{
		open_redir_file(&fd, io_streams->outfile_name, O_WRONLY \
			| O_CREAT | O_TRUNC, 0644);
		dup2_redir(fd, STDOUT_FILENO);
		close(fd);
	}
	if (io_streams->append_file_name)
	{
		open_redir_file(&fd, io_streams->append_file_name, O_WRONLY \
			| O_CREAT | O_APPEND, 0644);
		dup2_redir(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	set_io_streams(t_cmd_block *cmd)
{
	t_io_streams_list	*io_streams;

	if (!cmd)
		return ;
	io_streams = cmd->io_streams;
	while (io_streams)
	{
		handle_redir(io_streams);
		io_streams = io_streams->next;
	}
}
