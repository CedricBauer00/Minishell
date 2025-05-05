#include "execution.h"

t_shell	*init_shell_struct(t_gc_list **gc_lst)
{
	t_shell	*shell;

	shell = do_alloc(gc_lst, sizeof(t_shell), TYPE_SINGLE_PTR, "shell");
	if (!shell)
		return (NULL);
	shell->cur_dir = NULL;
	shell->old_dir = NULL;
	shell->heredoc_fd = -1;
	shell->my_envp = NULL;
	shell->pids = NULL;
	shell->last_status_exit = 0;
	return shell;
}

t_pipe	*init_pipe(t_gc	*gc)
{
	t_pipe	*p_pipe;

	p_pipe = do_alloc(&gc->temp, sizeof(t_pipe), TYPE_SINGLE_PTR, "pipe_list");
	if (!p_pipe)
		return (NULL);
	p_pipe->pipefd = do_alloc(&gc->temp, sizeof(int) * 2, TYPE_SINGLE_PTR, "pipefd");
	if (!p_pipe->pipefd)
		return (NULL);
	if (pipe(p_pipe->pipefd) == -1)
	{
		perror("init_pipe()");
		//gc_free(&gc_lst);
		return (NULL);
	}
	
	// p_pipe->next = NULL;
	// p_pipe->prev = NULL;
	return p_pipe;
}

t_io_streams_list	*init_io_stream_struct(t_gc *gc)
{
	t_io_streams_list	*io_streams_lst;

	io_streams_lst = do_alloc(&gc->temp, sizeof(t_io_streams_list), TYPE_SINGLE_PTR, "io_streams_list");
	if (!io_streams_lst)
		return (NULL);
	io_streams_lst->infile_name = NULL;
	io_streams_lst->outfile_name = NULL;
	io_streams_lst->fd_in_file = -1;
	io_streams_lst->fd_out_file = -1;
	io_streams_lst->append_file_name = NULL;

	io_streams_lst->heredoc_eof = NULL;
	io_streams_lst->heredoc_file = NULL;
	
	io_streams_lst->fd_org_read = 0;
	io_streams_lst->fd_in_file = 0;

	io_streams_lst->next = NULL;
	return (io_streams_lst);
}

t_cmd_block *init_command_struct(t_gc *gc)
{
	t_cmd_block	*cmd;

	cmd = do_alloc(&gc->temp, sizeof(t_cmd_block), TYPE_SINGLE_PTR, "cmd_block");
	if (!cmd)
		return (NULL);
	//cmd->value = NULL;
	cmd->built_in = NULL;
	cmd->is_built_in = false;
	cmd->is_external_cmd = false;
	//cmd->heredoc_fd = -1;
	cmd->args = NULL;
	//cmd->cmd_flags = NULL;
	cmd->io_streams = NULL;
	cmd->pipe = NULL;
	cmd->prev_read_end_fd = -1;
	cmd->cur_fd_write_end = -1;
	cmd->next = NULL;
	cmd->prev = NULL;
	//cmd->type = TOKEN_NONE;
	return (cmd);
}

//after using this function at the beginning, this func ensures that the 'shell' is no longer 'null'
//because it has already been initialized as a static var.
//static var is initialized just only at once when it called and assigned in data area.
t_shell	*get_shell(void)
{
	static t_shell	*shell = NULL;
	t_gc	*gc;

	gc = get_gc();
	if (shell == NULL)
	{
		shell = init_shell_struct(&gc->shell);
	}
	return (shell);
}

t_cmd_block	*get_cmd_block(void)
{
	static t_cmd_block	*cmd = NULL;
	t_gc	*gc;

	gc = get_gc();
	if (cmd == NULL)
	{
		cmd = init_command_struct(gc);
	}
	return (cmd);
}
