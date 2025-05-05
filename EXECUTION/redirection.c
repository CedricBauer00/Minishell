/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:17:26 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/05 14:17:27 by jisokim2         ###   ########.fr       */
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
		return -1;
	}
	fprintf(stderr, YELLOW"[pid %d] dup2(io_streams->fd_in_file : %d, STDIN_FILENO : %d)\n"DEFAULT,getpid(),fd, STDIN_FILENO);
	close(fd);
	fprintf(stderr, YELLOW"[pid %d] close(%d)\n"DEFAULT, getpid(), fd);
	return 1;
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
	fprintf(stderr, YELLOW"[pid %d] dup2(io_streams->fd_out_file : %d, STDOUT_FILENO : %d)\n"DEFAULT, getpid(), fd, STDOUT_FILENO);
	close(fd);
	fprintf(stderr, YELLOW"[pid %d] close(%d)\n"DEFAULT, getpid(), fd);
	return 1;
}

int	in_redir_file_open(t_io_streams_list *io_streams, char *in_filename)
{
	int		fd;
	fd = open(in_filename, O_RDONLY);
	if (fd == -1)
	{
		perror(RED" < file open faield\n"DEFAULT);
		return -1;
	}
	io_streams->fd_in_file = fd;
	fprintf(stderr, YELLOW"[pid %d] open() in_filename %s, io_streams->fd_in_filename,  %d)\n"DEFAULT,getpid(),in_filename, fd);
	return 1;
}

int	out_redir_file_open(t_io_streams_list *io_streams, char *out_filename)
{
	int		fd;
	fd = open(out_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		perror(RED" > file open faield\n"DEFAULT);
		return (-1);
	}
	io_streams->fd_out_file = fd;
	fprintf(stderr, YELLOW"[pid %d]open() out_filename : %s, io_streams->fd_out_filename, %d)\n"DEFAULT, getpid(), out_filename, fd);
	return 1;
}

int	append_redir_file_open(t_io_streams_list *io_streams, char *appned_file_name)
{
	int		fd;
	fd = open(appned_file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(RED" > file open faield\n"DEFAULT);
		return (-1);
	}
	io_streams->fd_out_file = fd;
	fprintf(stderr, YELLOW"[pid %d]open() appned_file_name : %s, fd_out_filename, %d)\n"DEFAULT, getpid(), appned_file_name, fd);
	return 1;
}

int	handle_re_dir(t_cmd_block *cmd_block)
{
	if(cmd_block->io_streams->fd_in_file)
		re_dir_in(cmd_block->io_streams);
	if (cmd_block->io_streams->fd_out_file)
		re_dir_out(cmd_block->io_streams);
	return 0;
}

void	set_io_streams(t_cmd_block *cmd)	
{
	t_io_streams_list *io_streams;
	
	if(!cmd)
		return ;
	io_streams = cmd->io_streams;
	while (io_streams)
	{
		if (io_streams->infile_name)
		{
			in_redir_file_open(io_streams, io_streams->infile_name);
			re_dir_in(io_streams);
		}
		if (io_streams->outfile_name)
		{
			out_redir_file_open(io_streams, io_streams->outfile_name);
			re_dir_out(io_streams);
		}
		if (io_streams->append_file_name)
		{
			append_redir_file_open(io_streams, io_streams->append_file_name);
			re_dir_out(io_streams);
		}
		io_streams = io_streams->next;
	}
}



// void	set_io_streams(t_cmd_block *cmd)	
// {
// 	t_io_streams_list *cur;
// 	t_io_streams_list *fd_in_redir;
// 	t_io_streams_list *fd_out_redir;
// 	t_io_streams_list *fd_append;

// 	if(!cmd)
// 		return ;
// 	cur = cmd->io_streams;
// 	while (cur)
// 	{
// 		if (cur->infile_name)
// 		{
// 			fd_in_redir = cur;
// 		}
// 		if (cur->outfile_name)
// 		{
// 			fd_out_redir = cur;
// 		}
// 		if (cur->append_file_name)
// 		{
// 			fd_append = cur;
// 		}
// 		cur = cur->next;
// 	}
// 	if (fd_in_redir)
// 	{
// 		in_redir_file_open(fd_in_redir, fd_in_redir->infile_name);
// 		re_dir_in(fd_in_redir);
// 	}
// 	if (fd_append)
// 	{
// 		append_redir_file_open(fd_append, fd_append->append_file_name);
// 		re_dir_out(fd_append);
// 	}
// 	else if (fd_out_redir)
// 	{
// 		out_redir_file_open(fd_out_redir, fd_out_redir->outfile_name);
// 		re_dir_out(fd_out_redir);
// 	}
// }