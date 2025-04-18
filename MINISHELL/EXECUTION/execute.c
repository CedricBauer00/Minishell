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

void 	is_executable_cmd(t_cmd_block *cmd_block, t_gc_list *gc_lst)
{
	char	**cmd_flags;
	char	**splitted_path;
	t_shell *shell;
	bool 	is_executable;

	is_executable = false;
	shell = get_shell();
	cmd_flags = ft_split(cmd_block->args, ' ');
	if (!cmd_flags)
	{
		//todo error handle;
	}
	//cmd_flags =>>> [ls] [-l] [-a] [-k]
	char *path = my_getenv(shell->my_envp, "PATH", 4, gc_lst);
	if (!path)
	{
		//todo error handle
	}

	splitted_path = ft_split(path, ':');
	/*
		usr/local/bin
		usr/bin
		sbin/
	*/

	int i = 0;
	while(splitted_path[i])
	{
		if (splitted_path[i] == NULL)
			break;
		/*
		usr/local/bin/
		usr/bin/
		sbin/
		*/
		char *attach_slash_to_cmd = ft_strjoin(splitted_path[i], "/");
		if (!attach_slash_to_cmd)
		{
			//todo error handle
		}
		/*
		usr/local/bin/[cmd_flags[0]]
		usr/bin/[cmd_flags[0]]
		sbin/[cmd_flags[0]]
		*/
		char *cmd_path = ft_strjoin(attach_slash_to_cmd, cmd_flags[0]);
		if (!cmd_path)
		{
			//todo error handle
		}

		if (access(cmd_path, F_OK | X_OK) == -1)
		{
			//todo error handle
			i++;
			continue;
		}
		else
		{
			printf(GREEN"there is file\n"DEFAULT);
			is_executable = true; 
		}
		if (is_executable == true)
		{
			fprintf(stderr, YELLOW"execve() %s\n"DEFAULT, cmd_path);
			if (execve(cmd_path , cmd_flags, shell->my_envp) == -1)
			{
				exit(1);
			}
		}
		else
		{
			fprintf(stderr, RED"not found cmd_path %s"DEFAULT, cmd_path);
			exit(1);
		}
	}
	exit(0);
}

void	execute(t_cmd_block *cmd_block, t_gc_list *gc_lst, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	pid;

	//t_shell *shell = get_shell();
	cur = cmd_block;
	if (is_validate_cmd_block(cur) == false)
	{
		//todo all free
		perror(RED"non valid cmd\n"DEFAULT);
		//exit(1);
	}
	if (cur->io_streams->heredoc_eof)
	{
		process_heredoc(cur->io_streams);
		if (dup2(cur->io_streams->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror(RED"dup2() heredocfd failed in validate_cmd_block()"DEFAULT);
			//todo free.
			//exit(1);
		}
	}
	while(cur)
	{
		//ERROR CUR->NEXT 
		//[CAT - E] | [WC] in der fall, fuehrt es nicht naechsten node aus, also [wc] geht nicht
		//weil es gibt keine node nach dem wc!
		if (cur->next)
		{
			add_pipe(&cur, gc_lst);
			fprintf(stderr, YELLOW"p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT, cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
			//cur->child_pids = do_alloc(&gc_lst, sizeof(pid_t), TYPE_SINGLE_PTR, "child PID");
			pid = fork();
			if (pid == 0)
			{
				set_io_streams(cur);
				processing_pipe(cur, shell, gc_lst);
				
				// fprintf(stderr, YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				// 	cmd->pipe->pipefd[0], cmd->pipe->pipefd[1]);
				if (cur->built_in)
				{
					//execute_builtin();
					printf(RED"it is builtin we need to implement it\n"DEFAULT);
					exit(0);
				}
				if (cur->args)
				{
					is_executable_cmd(cur, gc_lst);
				}
				//wenn heredoc in child prozess ausfuehrt dann muss es hier recover werden.
			}
			else
			{
				close_pipefd(cur);
				int status;
				pid_t child_pid = wait4(-1, &status, 0 ,NULL);
				if (child_pid > 0)
				{
					if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
					{
						printf(GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
						shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
					}
					else if (WIFSIGNALED(status))
					{
						printf(RED "terminated by signal %d (%s)\n" DEFAULT, WTERMSIG(status), strsignal(WTERMSIG(status)));
						shell->last_status_exit = 128 + WTERMSIG(status);
					}
				}
			}
			if()
			{
				
			}
		}
		cur = cur->next;
	}

	//todo single command!
	if(cur && !cur->prev && !cur->next)
	{
		fprintf(stderr,"isin here \n");
		if (cur->built_in)
		{
			set_io_streams(cur);
			//execute_builtin();
		}
		else if(cur->args)
		{
			pid = fork();
			fprintf(stderr, YELLOW"fork() : %d \n"DEFAULT, pid);
			if (pid == 0)
			{
				set_io_streams(cur);
				is_executable_cmd(cur, gc_lst);
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
			}
		}
	}
}

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

t_token *create_token(t_token_type type, char *value)
{
	t_token *new = malloc(sizeof(t_token));
	if (!new)
		return NULL;
	new->type = type;
	new->value = ft_strdup(value);
	new->prev = NULL;
	new->next = NULL;
	return new;
}

//cat < 1.txt < 2.txt | wc
int main(int ac, char *argv[], char *envp[])
{
	(void)ac;
	(void)argv;
	t_token *t1 = create_token(TOKEN_ARGS, "cat -e");
	t_token *t2 = create_token(TOKEN_REDIRECT_IN, "<");
	t_token *t3 = create_token(TOKEN_FILE, "1.txt");
	t_token *t4 = create_token(TOKEN_REDIRECT_IN, "<");
	t_token *t5 = create_token(TOKEN_FILE, "2.txt");
	t_token *t6 = create_token(TOKEN_PIPE, "|");
	t_token *t7 = create_token(TOKEN_ARGS, "wc");
	t1->next = t2;
	t2->prev = t1;
	t2->next = t3;
	t3->prev = t2;
	t3->next = t4;
	t4->prev = t3;
	t4->next = t5;
	t5->prev = t4;
	t5->next = t6;
	t6->prev = t5;
	t6->next = t7;
	t7->prev = t6;
	t_gc	*gc = get_gc();
	// t_gc_list* gc_lst = init_gc_list();
	t_cmd_block *cmd_block_list = NULL;
	grouplize(t1, &cmd_block_list, gc->temp);
	if(cmd_block_list)
	{
		if (cmd_block_list->built_in)
			printf("cmd_block_list->built_in %s\n", cmd_block_list->built_in);
		if (cmd_block_list->args)
			printf("cmd_block_list->args %s\n", cmd_block_list->args);

		printf("cmd_block_list->io_streams->infile_name : %s\n", cmd_block_list->io_streams->infile_name);
		printf("cmd_block_list->io_streams->next->infile_name : %s\n", cmd_block_list->io_streams->next->infile_name);
		fprintf(stderr, "cmd->io_streams->heredoc_eof %s\n", cmd_block_list->io_streams->heredoc_eof);
		printf("cmd_block_list->io_streams->outfile_name : %s\n", cmd_block_list->io_streams->outfile_name);
		printf("cmd_block_list->io_streams->next->outfile_name :%s\n", cmd_block_list->io_streams->next->outfile_name);
		printf("cmd_block_list->pipe : %p\n", cmd_block_list->pipe);
	}
	if (cmd_block_list->next)
	{
		printf("cmd_block_list->next->cmd : %p, %s\n",cmd_block_list->next, cmd_block_list->next->args);
	}
	else
	{
		printf("no pipe\n");
	}
	t_shell *shell = get_shell();
	shell ->my_envp = copy_envp(gc->shell, envp);
	execute(cmd_block_list, gc->temp, shell);
	gc_free(gc);
	return 0;
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
