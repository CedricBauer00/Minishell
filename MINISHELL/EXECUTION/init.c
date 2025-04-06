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

t_pipe *init_pipe_list(t_gc_list *gc_lst)
{
	t_pipe *pipe;
	pipe = do_alloc(gc_lst, sizeof(t_pipe), TYPE_SINGLE_PTR);
	if (!pipe)
	{
		return (NULL);
	}
	pipe->pipefd = NULL;
	pipe->prev_read_end_fd = -1;
	pipe->cur_fd_write_end = -1;
	pipe->next = NULL;
	pipe->prev = NULL;
	return pipe;
}

t_token *init_command_struct(t_gc_list *gc_lst)
{
	t_token *cmd;
	cmd = do_alloc(gc_lst, sizeof(t_token), TYPE_SINGLE_PTR);
	if (!cmd)
	{
		return (NULL);
	}
	cmd->value = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->infile_name = NULL;
	cmd->outfile_name = NULL;
	cmd->pipe = do_alloc(gc_lst, sizeof(t_pipe), TYPE_SINGLE_PTR);
	if (!cmd->pipe)
	{
		all_free(&gc_lst);
		return NULL;
	}
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->type = TOKEN_NONE;
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
