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


//cmd block has only just one child process
/*
	[ls -l] | [wc -l] | [cat]
*/

void	execute_cmd(t_cmd_block *cmd_block)
{
	char	**cmd_flags;
	
	cmd_flags = ft_split(cmd_block->args, ' ');
	
}
void	execute(t_cmd_block *cmd_block, t_gc_list *gc_lst, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	pid;

	t_shell *shell = get_shell();
	cur = cmd_block;
	if (validate_cmd_block(cur) == 0)
	{
		//todo all free
		perror(RED"non valid cmd"DEFAULT);
		//exit(1);
	}
	while(cur)
	{
		//TODO denk mal ueber if Bedingung nach cur->next || oder cur->next->type == TOKEN_PIPE	weil wenn es naechste node gibt, bedeutet das, es gibt pipe auch!
		if (cur->next)
		{
			add_pipe(&cur, gc_lst);
			//cur->child_pids = do_alloc(&gc_lst, sizeof(pid_t), TYPE_SINGLE_PTR, "child PID");
			pid = fork();
			if (pid == 0)
			{
				processing_pipe(cur, shell, gc_lst);
				set_io_streams(cur);
				// fprintf(stderr, YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				// 	cmd->pipe->pipefd[0], cmd->pipe->pipefd[1]);
				if (cur->built_in)
				{
					execute_builtin();
				}
				if (cur->args)
				{
					execute_cmd();
				}
				//wenn heredoc in child prozess ausfuehrt dann muss es hier recover werden.
				exit(0);
			}
			else
			{
				int status;
				pid_t child_pid = wait4(-1, &status, 0 ,NULL);
				if (child_pid > 0)
				{
					if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
					{
						printf(GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
						shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
					}
					//todo stduy about that 
					/*
						if child process exited by signal also abnormal
						128 + signal_number
						total siganl number is 31
						max signal return number is 159
					*/
					else if (WIFSIGNALED(status))
					{
						printf(RED "terminated by signal %d (%s)\n" DEFAULT, WTERMSIG(status), strsignal(WTERMSIG(status)));
						shell->last_status_exit = 128 + WTERMSIG(status);
					}
				}
				close_pipefd(cur);
			}
		}

		//single command!
		else if(!cur->prev && !cur->next)
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
					execute_cmd();
				}
				else
				{
					
				}
			}
		}
		cur = cur->next;
	}
}

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}
// void	execute(t_cmd_block *cmd_block, t_gc_list *gc_list)
// {
// 	int	i;
// 	t_shell *shell;

// 	i = 0;
// 	shell = get_shell();
// 	while(shell->my_envp[i])
// 	{
// 		if (access())
// 	}
// }
