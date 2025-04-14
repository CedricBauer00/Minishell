# include "../INCLUDE/main.h"

//TODO 
//Denk mal nach, wie ich die PIDs behandeln soll.
/*
	IF (CUR->NEXT == PIPE)
		CHECK IS IT FIRST OR MIDDLE OR LAST?
	PID = FORK();
	IF(PID == 0)
	{
		IF (FIRST)
			PROCESS FIRST PIPE
		IF (MIDDLE)
			PROCESS MIDDLE PIPE
		IF (LAST)
			PROCESS LAST PIPE
	}

	ex )
	if (cur->pipe->stage == NONE && is_first_pipe_cmd(cur->pipe->pipefd))
	{
		cur->pipe->stage = FIRST;
	}
	else if (cur->pipe->stage == FIRST && is_middle_pipe_cmd(cur->pipe->prev_read_end_fd,cur->pipe->cur_fd_write_end))
	{
		cur->pipe->stage = MIDDLE;
	}
	else if (cur->pipe->stage == MIDDLE && is_last_pipe_cmd(cur->pipe->prev_read_end_fd,cur->pipe->cur_fd_write_end))
	{
		cur->pipe->stage = LAST;
	}
*/

void	set_io_streams(t_cmd_block *cmd)	
{
	t_io_streams_list *io_streams;

	if(!cmd)
		return ;
	io_streams = cmd;
	while (io_streams)
	{
		if (io_streams->fd_in_file && io_streams->infile_name)
		{
			re_dir_in(io_streams);
		}
		if (io_streams->fd_out_file && io_streams->outfile_name)
		{
			re_dir_out(io_streams);
		}
		io_streams = io_streams->next;
	}
}

bool	is_first_pipe(t_cmd_block *cmd)
{
	if (!cmd->prev && cmd->next)
		return true;
	else
		return false;
}

bool	is_middle_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && cmd->next)
		return true;
	else
		return false;
}

bool	is_last_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && !cmd->next)
		return true;
	else
		return false;
}

void	set_redirection(t_cmd_block *cmd_block, t_gc_list *gc_lst, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	pid;

	t_shell *shell = get_shell();
	cur = cmd_block;
	while(cur)
	{
		//TODO denk mal ueber if Bedingung nach cur->next || oder cur->next->type == TOKEN_PIPE	weil wenn es naechste node gibt, bedeutet das, es gibt pipe auch!
		if (cur->next)
		{
			add_pipe(&cur, gc_lst);
			pid = fork();
			if (pid == 0)
			{
				processing_pipe(cur, shell, gc_lst);
				// if (cur->io_streams)
				// {
				// 	t_io_streams_list *io_streams = cur;
				// 	while(io_streams->fd_in_file && io_streams->infile_name)
				// 	{
				// 		re_dir_in(io_streams);
				// 		io_streams = io_streams->next;
				// 	}
				// 	while(io_streams->fd_out_file && io_streams->outfile_name)
				// 	{
				// 		re_dir_out(io_streams);
				// 		io_streams = io_streams->next;
				// 	}
				// }
				set_io_streams(cur);
				// fprintf(stderr, YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				// 	cmd->pipe->pipefd[0], cmd->pipe->pipefd[1]);
				if (cur->built_in || cur->cmd)
				{
					
				}
				if (cur->cmd)
				{

				}
			}
			else
			{
				close_pipefd(cur);
			}
		}
		//single command!

		else if(!cur->next && !cur->pipe)
		{
			if (cur->built_in)
			{
				set_io_streams(cur);
				execute_builtin();
			}
			else if(cur->cmd)
			{
				pid = fork();
				if (pid == 0)
				{
					set_io_streams(cur);
				}
				else
				{

				}
			}
		}
		cur = cur->next;
	}
}

