# include "../INCLUDE/main.h"

//todo both redirection must be called in child!
// todo need to change if 

int	re_dir_in(t_cmd_block *cmd_block)
{
	int	fd;

	fd = cmd_block->io_streams->fd_in_file;
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
int	re_dir_out(t_cmd_block *cmd_block)
{
	int	fd;

	fd = cmd_block->io_streams->fd_out_file;
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
		re_dir_in(cmd_block);
	if (cmd_block->io_streams->fd_out_file)
		re_dir_out(cmd_block);
	return 0;
}
