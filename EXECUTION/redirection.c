/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:26 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/14 14:31:15 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_redir_file(char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = 0;
	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", filename, strerror(errno));
		return (fd);
	}
	return (fd);
}

static int	dup2_redir(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		printf("minishell: dup2(%d: %d), %s\n", oldfd, newfd, strerror(errno));
		return (-1);
	}
	return (0);
}

static int	hoo(char *filename, int stdfd, int flags, mode_t mode)
{
	int	fd;

	fd = 0;
	fd = open_redir_file(filename, flags, mode);
	if (fd < 0)
		return (-1);
	if (dup2_redir(fd, stdfd) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_redir(t_io_list	*io_streams)
{
	if (!io_streams)
		return (0);
	if (io_streams->infile_name)
	{
		if (hoo (io_streams->infile_name, STDIN_FILENO, O_RDONLY, 0) < 0)
			return (-1);
	}
	if (io_streams->outfile_name)
	{
		if (hoo (io_streams->outfile_name, \
			STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0)
			return (-1);
	}
	if (io_streams->append_file_name)
	{
		if (hoo (io_streams->append_file_name, \
			STDOUT_FILENO, O_WRONLY | O_CREAT | O_APPEND, 0644) < 0)
			return (-1);
	}
	return (0);
}

int	set_io_streams(t_cmd_block *cmd)
{
	t_io_list	*io_streams;

	if (!cmd || !cmd->io_streams)
		return (-1);
	io_streams = cmd->io_streams;
	while (io_streams)
	{
		if (handle_redir(io_streams) < 0)
			return (-1);
		io_streams = io_streams->next;
	}
	return (0);
}
