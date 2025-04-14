# include "../INCLUDE/main.h"

t_shell	*init_shell_struct(void)
{
	t_shell *shell;
	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		return (NULL);
	}
	shell->cur_dir = NULL;
	shell->old_dir = NULL;
	shell->my_envp = NULL;
	shell->last_status_exit = 0;
	return shell;
}

t_pipe *init_pipe(t_gc_list *gc_lst)
{
	t_pipe *p_pipe;
	p_pipe = do_alloc(gc_lst, sizeof(t_pipe), TYPE_SINGLE_PTR);
	if (!p_pipe)
	{
		return (NULL);
	}
	p_pipe->pipefd = do_alloc(gc_lst, sizeof(int) * 2, TYPE_SINGLE_PTR);
	if (!p_pipe->pipefd)
	{
		return (NULL);
	}
	if (pipe(p_pipe->pipefd) == -1)
	{
		perror("init_pipe()");
		all_free(&gc_lst);
		return (NULL);
	}
	p_pipe->stage = NONE;
	p_pipe->prev_read_end_fd = -1;
	p_pipe->cur_fd_write_end = -1;
	// p_pipe->next = NULL;
	// p_pipe->prev = NULL;
	return p_pipe;
}

t_io_streams_list *init_io_stream_struct(t_gc_list *gc_lst)
{
	t_io_streams_list	*io_streams_lst;
	io_streams_lst = do_alloc(gc_lst, sizeof(t_io_streams_list), TYPE_SINGLE_PTR);
	if (!io_streams_lst)
	{
		return (NULL);
	}
	io_streams_lst->infile_name = NULL;
	io_streams_lst->outfile_name = NULL;
	io_streams_lst->fd_in_file = 0;
	io_streams_lst->fd_in_file = 0;
	io_streams_lst->next = NULL;
	return (io_streams_lst);
}

t_cmd_block *init_command_struct(t_gc_list *gc_lst)
{
	t_cmd_block *cmd;
	cmd = do_alloc(gc_lst, sizeof(t_cmd_block), TYPE_SINGLE_PTR);
	if (!cmd)
	{
		return (NULL);
	}
	//cmd->value = NULL;
	cmd->built_in = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->io_streams = NULL;
	cmd->pipe = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	//cmd->type = TOKEN_NONE;
	return (cmd);
}

//after using this function at the beginning, this func ensures that the 'shell' is no longer 'null'
//because it has already been initialized as a static var.
//static var is initialized just only at once when it called and assigned in data area.
t_shell *get_shell(void)
{
	static t_shell *shell = NULL;
	if (shell == NULL)
	{
		shell = init_shell_struct();
	}
	return (shell);
}
