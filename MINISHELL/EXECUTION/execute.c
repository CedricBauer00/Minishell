# include "../INCLUDE/main.h"

//TODO 
//Denk mal nach, wie ich die PIDs behandeln soll.

void	set_redirection(t_cmd_block *cmd, t_gc_list *gc_lst, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	*pid;

	t_shell *shell = get_shell();
	cur = cmd;
	while(cur)
	{
		//TODO denk mal ueber if Bedingung nach cur->next || oder cur->next->type == TOKEN_PIPE	weil wenn es naechste node gibt, bedeutet das, es gibt pipe auch!
		if (cur->next)
		{
			add_pipe(&cur, gc_lst);
			pid = fork();
			if (pid == 0)
			{
				if (is_first_pipe_cmd(cur->pipe->pipefd))
					first_pipe_cmd(cur, shell, gc_lst);
				if (is_middle_pipe_cmd(cur->pipe->prev_read_end_fd, cur->pipe->cur_fd_write_end))
					middle_pipe_cmd(cur, shell, gc_lst);
				if (is_last_pipe_cmd(cur->pipe->prev_read_end_fd, cur->pipe->cur_fd_write_end))
					last_pipe_cmd(cur, shell);
				if (cur->io_streams)
				{
					while(cur->io_streams->fd_in_file && cur->io_streams->infile_name)
					{
						re_dir_in(cur);
						cur->io_streams = cur->io_streams->next;
					}
					while(cur->io_streams->fd_out_file && cur->io_streams->outfile_name)
					{
						re_dir_out(cur);
						cur->io_streams = cur->io_streams->next;
					}
				}
			}
			
			fprintf(stderr, YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				cmd->pipe->pipefd[0], cmd->pipe->pipefd[1]);
		}

		//single command!
		else if(!cur->next)
		{

		}
		
	
		cur = cur->next;
	}
}

