/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:26 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/17 13:57:46 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	open_redir_file_dup2(char *filename, int stdfd,
	int flags, mode_t mode)
{
	int		fd;
	t_gc	*gc;

	gc = get_gc();
	fd = 0;
	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		gc_free(gc);
		exit(1);
	}
	if (dup2(fd, stdfd) == -1)
	{
		close(fd);
		gc_free(gc);
		exit(1);
	}
	close(fd);
}

static void	hoo(char *filename, int stdfd, int flags, mode_t mode)
{
	open_redir_file_dup2(filename, stdfd, flags, mode);
}

static void	handle_redir(t_io_list	*io_streams)
{
	if (!io_streams)
		return ;
	if (io_streams->infile_name)
	{
		hoo (io_streams->infile_name, STDIN_FILENO, O_RDONLY, 0);
	}
	if (io_streams->outfile_name)
	{
		hoo (io_streams->outfile_name, \
			STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (io_streams->append_file_name)
	{
		hoo (io_streams->append_file_name, \
			STDOUT_FILENO, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
}

void	set_io_streams(t_cmd_block *cmd)
{
	t_io_list	*io_streams;

	if (!cmd || !cmd->io_streams)
		return ;
	io_streams = cmd->io_streams;
	while (io_streams)
	{
		handle_redir(io_streams);
		io_streams = io_streams->next;
	}
}
