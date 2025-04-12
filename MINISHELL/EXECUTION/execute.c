# include "../INCLUDE/main.h"

//TODO 
//Denk mal nach, wie ich die PIDs behandeln soll.

void	execute_cmds(t_cmd_block *cmd, t_gc_list *gc_lst, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	*pid;

	cur = cmd;
	while(cur)
	{
		//TODO denk mal ueber if Bedingung nach cur->next || oder cur->next->type == TOKEN_PIPE	weil wenn es naechste node gibt,heisst das, es gibt pipe auch!
		if (cur->next && cur->next->type == TOKEN_PIPE) 
		{
			add_pipe(&cur, gc_lst);
			fprintf(stderr, YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				cmd->pipe->pipefd[0], cmd->pipe->pipefd[1]);
		}
		if (cur->type == TOKEN_BUILT_IN)
		{
			
		}
		if (cur->type == TOKEN_REDIRECT_IN)
		{

		}
		if (cur->type == TOKEN_REDIRECT_OUT)
		{

		}
		if (cur->type == TOKEN_CMD)
		{
			pid = fork();
			{
				execve(cur->cmd, )
				// if (pid == 0)
				// {
				// 	if (cur->next && cur->pipe->prev_read_end_fdb == -1 && cur->pipe->cur_fd_write_end == -1)
				// 		first_pipe_cmd(token, shell, gc_lst);
				// 	if (cur->next && cur->pipe->prev_read_end_fd != -1 && cur->pipe->cur_fd_write_end != -1)
				// 		middle_pipe_cmd(token, shell, gc_lst);
				// 	if (cur->next && cur->pipe->prev_read_end_fd != -1 && cur->pipe->cur_fd_write_end == -1)
				// 		last_pipe_cmd(token, shell);
				// }
				// else
				// {
					
				// }
			}
		}
		cur = cur->next;
	}
}

