#include "execution.h"

//todo both redirection must be called in child!
// todo need to change if 

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

//todo need to change if 
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

//todo here if heredoc then just run it here 
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
		// if (io_streams->heredoc_fd > 0)
		// {
		// 	dup2(cmd->io_streams->heredoc_fd, STDIN_FILENO);
		// 	fprintf(stderr ,RED"[pid %d] dup2(HEREDOC: %d, %d)\n"DEFAULT, getpid(), cmd->io_streams->heredoc_fd, STDIN_FILENO);
		// }
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

/*
		   3 		4
	cat << eof < file.1

	4
*/