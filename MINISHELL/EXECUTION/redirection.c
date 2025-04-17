# include "../INCLUDE/main.h"

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
	close(fd);
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
	close(fd);
	return 1;
}

int	in_redir_file_open(t_cmd_block *cmd_block, char *in_filename)
{
	int		fd;
	fd = open(in_filename, O_RDONLY);
	if (fd == -1)
	{
		perror(RED" < file open faield\n"DEFAULT);
		return -1;
	}
	cmd_block->io_streams->fd_in_file = fd;
	return 1;
}

int	out_redir_file_open(t_cmd_block *cmd_block, char *out_filename)
{
	int		fd;
	fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(RED" > file open faield\n"DEFAULT);
		return (-1);
	}
	cmd_block->io_streams->fd_out_file = fd;
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
		if (io_streams->fd_in_file && io_streams->infile_name)
		{
			re_dir_in(io_streams);
		}
		if (io_streams->heredoc_fd)
		{
			//todo call heredoc
			dup2(cmd->io_streams->heredoc_fd, STDIN_FILENO);
		}
		if (io_streams->fd_out_file && io_streams->outfile_name)
		{
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