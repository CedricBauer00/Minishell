/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:26 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 16:30:29 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	re_dir_in(t_io_streams_list *io_streams)
{
	int	fd;

	fd = io_streams->fd_in_file;
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror(RED"dup2() for '<' error\n"DEFAULT);
		close(fd);
		return (-1);
	}
	close(fd);
	return (1);
}

int	re_dir_out(t_io_streams_list *io_streams)
{
	int	fd;

	fd = io_streams->fd_out_file;
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror(RED"dup2() for '>' error\n"DEFAULT);
		close(fd);
		return (-1);
	}
	close(fd);
	return (1);
}

int	in_redir_file_open(t_io_streams_list *io_streams, char *in_filename)
{
	int	fd;

	fd = open(in_filename, O_RDONLY);
	if (fd == -1)
	{
		perror(RED" < file open faield\n"DEFAULT);
		return (-1);
	}
	io_streams->fd_in_file = fd;
	return (1);
}

int	out_redir_file_open(t_io_streams_list *io_streams, char *out_filename)
{
	int	fd;

	fd = open(out_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		perror(RED" > file open faield"DEFAULT);
		return (-1);
	}
	io_streams->fd_out_file = fd;
	return (1);
}

int	append_redir_file_open(t_io_streams_list *io_streams, char *appned_file_name)
{
	int	fd;

	fd = open(appned_file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(RED" >> file open faield"DEFAULT);
		return (-1);
	}
	io_streams->fd_out_file = fd;
	return (1);
}

int	handle_re_dir(t_cmd_block *cmd_block)
{
	if (cmd_block->io_streams->fd_in_file)
		re_dir_in(cmd_block->io_streams);
	if (cmd_block->io_streams->fd_out_file)
		re_dir_out(cmd_block->io_streams);
	return 0;
}

static void	handle_redir_error()
{
	t_gc	*gc;
	
	gc = get_gc();
	gc_free(gc);
	exit(1);
}

void	set_io_streams(t_cmd_block *cmd)	
{
	t_io_streams_list	*io_streams;
	//t_gc				*gc;

	//gc = get_gc();
	if (!cmd)
		return ;
	io_streams = cmd->io_streams;
	while (io_streams)
	{
		if (io_streams->infile_name)
		{
			if (in_redir_file_open(io_streams, io_streams->infile_name) == -1)
				handle_redir_error();
			if (re_dir_in(io_streams) == -1)
				handle_redir_error();
		}
		if (io_streams->outfile_name)
		{
			if (out_redir_file_open(io_streams, io_streams->outfile_name) == -1)
				handle_redir_error();
			if (re_dir_out(io_streams) == -1)
				handle_redir_error();
		}
		if (io_streams->append_file_name)
		{
			if (append_redir_file_open(io_streams, io_streams->append_file_name) == -1)
				handle_redir_error();
			if (re_dir_out(io_streams) == -1)
				handle_redir_error();
		}
		io_streams = io_streams->next;
	}
}

