# include "../INCLUDE/main.h"

//TODO 
//Denk mal nach, wie ich die PIDs behandeln soll.

void	execute_cmds(t_token *token, t_gc_list *gc_lst, t_shell *shell)
{
	t_token *cur;
	pid_t	*pid;

	cur = token;
	while(cur)
	{
		if (cur->next && cur->next->type == TOKEN_PIPE)
		{
			add_pipe(&cur, gc_lst);
			fprintf(stderr, YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				token->pipe->pipefd[0], token->pipe->pipefd[1]);
		}
		if (cur->type == TOKEN_BUILT_IN)
		{

		}
		if (cur->type == TOKEN_CMD)
		{
			pid = fork();
			{
				if (pid == 0)
				{
					if (cur->prev && cur->prev->type == TOKEN_REDIRECT_IN)
						handle_re_dir(cur);
					if (cur->next && cur->next->type == TOKEN_REDIRECT_OUT)
						handle_re_dir(cur);

					if (cur->pipe && cur->pipe->prev_read_end_fd == -1 && cur->pipe->cur_fd_write_end == -1)
						first_pipe_cmd(token, shell, gc_lst);
					if (cur->pipe && cur->pipe->prev_read_end_fd != -1 && cur->pipe->cur_fd_write_end != -1)
						middle_pipe_cmd(token, shell, gc_lst);
					if (cur->pipe && cur->pipe->prev_read_end_fd != -1 && cur->pipe->cur_fd_write_end == -1)
						last_pipe_cmd(token, shell);
					execve(cur->cmd, cur->args, shell->my_envp);
					exit(0);
				}
				else
				{
					
				}
			}
		}
		cur = cur->next;
	}
}