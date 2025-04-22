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

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void	wait_for_child_and_update_status(t_cmd_block *cur)
{
	t_shell *shell = get_shell();
	int status;
	pid_t child_pid = wait4(-1, &status, 0 ,NULL);
	fprintf(stderr ,BLUE"parent got this from wait4() child_pid : %d\n"DEFAULT, child_pid);
	if (child_pid > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		else if (WIFSIGNALED(status))
		{
			fprintf(stderr, RED"terminated by signal %d (%s)\n" DEFAULT, WTERMSIG(status), strsignal(WTERMSIG(status)));
			shell->last_status_exit = 128 + WTERMSIG(status);
		}
	}
}

//todo change name becuz it doenst make offset or maybe i can make it to have it offset 
int heredoc_fd_offset_and_redir(t_cmd_block *cur)
{
	if (!cur)
		return -1;
	fprintf(stderr, RED"in execute_child cur->io_streams->heredoc_fd: %d\n"DEFAULT, cur->io_streams->heredoc_fd);
	fprintf(stderr, RED"in execute_child STDIN_FILENO: %d, STDOUT_FILENO: %d\n"DEFAULT, STDIN_FILENO, STDOUT_FILENO);
	cur->io_streams->heredoc_fd = open("temp_heredoc" ,O_RDWR, 0644);
	if (cur->io_streams->heredoc_fd < 0)
	{
		return -1;
	}
	if (dup2(cur->io_streams->heredoc_fd, STDIN_FILENO) == -1)
	{
		close(cur->io_streams->heredoc_fd);
		unlink("temp_heredoc");
		return -1;
	}
	fprintf(stderr, RED"[pid %d] dup2(%d, %d)\n"DEFAULT, getpid(), cur->io_streams->heredoc_fd, STDIN_FILENO);
	close(cur->io_streams->heredoc_fd);
	unlink("temp_heredoc");
	return 1;
}

//todo mach die funktion fertig 
// int	execute_builtin(t_cmd_block *cur, t_gc_list *gc_lst, t_shell *shell)
// {
// 	char *temp = cur->built_in;
// 	char **argv = ft_split(temp, ' ');
// 	if ((strcmp(argv[0], "echo") == 0))
// 		ft_echo(argv, shell, gc_lst);
// 	if ((strcmp(argv[0], "cd") == 0))
// 		cd(argv, shell, gc_lst);
	
// }
	
void	single_cmd_execute(t_cmd_block *cur, pid_t pid, t_gc_list *gc_lst)
{
	if (!pid || !cur)
		return;
	if (cur->built_in)
	{
		if (cur->io_streams && cur->io_streams->heredoc_eof)
		{
			if (heredoc_fd_offset_and_redir(cur) == -1)
			{
				perror("errror");
				//todo free
				exit(1);
			}
		}
		set_io_streams(cur);
		//todo execute_builtin();
		//recover STDIN and STDOUT maybe. in hier? oder in der main_excute funktion? 
	}
	else if(cur->args) //if external cmd
	{
		pid = fork();
		fprintf(stderr, YELLOW" singlecmd fork() : %d \n"DEFAULT, pid);
		if (pid == 0)
		{
			if (cur->io_streams && cur->io_streams->heredoc_eof)
			{
				if (heredoc_fd_offset_and_redir(cur) == -1)
				{
					perror("errror");
					//todo free
					exit(1);
				}
			}
			set_io_streams(cur);
			is_executable_cmd(cur, gc_lst);
		}
		else
			wait_for_child_and_update_status(cur);
	}
}

void	execute_child(pid_t pid, t_cmd_block *cur, t_gc_list *gc_lst, t_shell *shell)
{
	if (pid == 0)
	{
		set_io_streams(cur);
		processing_pipe(cur, shell, gc_lst);
		if (cur->io_streams && cur->io_streams->heredoc_eof)
		{
			if (heredoc_fd_offset_and_redir(cur) == -1)
			{
				perror("errror");
				//todo free
				exit(1);
			}
		}
		if (cur->built_in)
		{
			printf(RED"it is builtin we need to implement it\n"DEFAULT);
			exit(0);
		}
		if (cur->args)
		{
			is_executable_cmd(cur, gc_lst);
		}
	}
}

// char	**get_splitted_path()
// {
// 	t_shell	*shell;

// 	shell = get_shell();
// 	char *path = find_var_in_env(shell->my_envp, "PATH", 4, gc_lst);
// 	if (!path)
// 	{
// 		//todo error handle
// 		return NULL;
// 	}
//	splitted_path = ft_split(path, ':');
// 	return splitted_path;
// }

// char	*get_cmd_path()
// {
// 	char	**splitted_path;
// 	t_shell *shell;

// 	shell = get_shell();
// 	splitted_path = get_splitted_path();
// 	int i = 0;
// 	while(splitted_path[i])
// 	{
// 		char *attach_slash_to_cmd = ft_strjoin(splitted_path[i], "/");
// 		if (!attach_slash_to_cmd)
// 		{
// 			//todo error handle
// 		}
// 		char *cmd_path = ft_strjoin(attach_slash_to_cmd, cmd_block->args[0]);
// 		if (!cmd_path)
// 		{
// 			//todo error handle
// 		}
// 		if (access(cmd_path, F_OK | X_OK) == 0)
// 		{
// 			if (execve(cmd_path , cmd_block->args, shell->my_envp) == -1)
// 			{
// 				fprintf(stderr, RED"error\n"DEFAULT);
// 				exit(1);
// 			}
// 			exit(0);
// 		}
// 		i++;
// 	}
// 	fprintf(stderr, RED"command not found: %s\n"DEFAULT, cmd_block->args[0]);
// 	exit(127);
//  }

void 	is_executable_cmd(t_cmd_block *cmd_block, t_gc_list *gc_lst)
{
	char	**splitted_path;
	t_shell *shell;
	bool 	is_executable;

	is_executable = false;
	shell = get_shell();
	char *path = find_var_in_env(shell->my_envp, "PATH", 4, gc_lst);
	if (!path)
	{
		//todo error handle
	}
	splitted_path = ft_split(path, ':');
	int i = 0;
	while(splitted_path[i])
	{
		char *attach_slash_to_cmd = ft_strjoin(splitted_path[i], "/");
		if (!attach_slash_to_cmd)
		{
			//todo error handle
		}
		char *cmd_path = ft_strjoin(attach_slash_to_cmd, cmd_block->args[0]);
		if (!cmd_path)
		{
			//todo error handle
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			if (execve(cmd_path , cmd_block->args, shell->my_envp) == -1)
			{
				fprintf(stderr, RED"error\n"DEFAULT);
				exit(1);
			}
			else
				exit(0);
		}
		i++;
	}
	fprintf(stderr, RED"command not found: %s\n"DEFAULT, cmd_block->args[0]);
	exit(127);
}


void	main_execute(t_cmd_block *cmd_block, t_gc_list *gc_lst, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	pid;

	//t_shell *shell = get_shell();
	cur = cmd_block;
	t_cmd_block *heredoc_check = cmd_block;
	if (is_validate_cmd_block(cur) == false)
	{
		//todo all free
		perror(RED"non valid cmd"DEFAULT);
		shell->last_status_exit = 1;
		exit(1);
	}

	while(heredoc_check)
	{
		if (heredoc_check->io_streams && heredoc_check->io_streams->heredoc_eof)
		{
			printf("heredoc_check->io_streams->heredoc_eof %s\n", heredoc_check->io_streams->heredoc_eof);
			process_heredoc(heredoc_check->io_streams);
			printf(RED"heredocfd %d\n"DEFAULT, heredoc_check->io_streams->heredoc_fd);
		}
		else
			break;
		heredoc_check = heredoc_check->next;
	}


	//memo if this single cmd
	if(cur && !cur->prev && !cur->next)
	{
		fprintf(stderr, RED"1\n"DEFAULT);
		single_cmd_execute(cur, pid, gc_lst);
	}

	while(cur)
	{
		if (cur->next)
		{
			add_pipe(&cur, gc_lst);
			fprintf(stderr, YELLOW"[pid %d] p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT,getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
			pid = fork();
			fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
			if (pid == 0)
				execute_child(pid, cur, gc_lst, shell);
			close_pipefd(cur);
		}

		//memo if last cmd
		else if(cur && cur->prev && !cur->next)
		{
			pid = fork();
			fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
			if (pid == 0)
				execute_child(pid, cur, gc_lst, shell);
			close_pipefd(cur);
		}
		cur = cur->next;
	}
	
	cur = cmd_block;
	wait_for_child_and_update_status(cur);
	prevent_zombie_process();
	fprintf(stderr, RED"-CHECK ORGINAL STDIN AND STDOUT-\n in execute_child STDIN_FILENO: %d, STDOUT_FILENO: %d\n"DEFAULT, STDIN_FILENO, STDOUT_FILENO);
}

//delete it later 
static t_token *create_token(t_token_type type, char *value)
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

//cat < 1 < 2 | cat -e >> 3
//cat < 1 < 2 | ls > 3
//cat < 1 < 2 | grep ehllo >> 4 >> 5
//cat < 1 < 2 | grep hello >> 2
//cat < 1 < 2 | cat -e >> 2 
//cat << eof << eof | cat -e >> 2
//cat < 1 < 2 | cat -e >> 2 | ls -a | cat -b -e
//ls -a -n -l < 1 -R


int main(int ac, char *argv[], char *envp[])
{
	(void)ac;
	(void)argv;
	t_token *t1 = create_token(TOKEN_ARGS, "cat");
	// t_token *t1_2 = create_token(TOKEN_ARGS, "-a");
	// t_token *t1_3 = create_token(TOKEN_ARGS, "-n");
	// t_token *t1_4 = create_token(TOKEN_ARGS, "-l");
	// t_token *t1_5 = create_token(TOKEN_REDIRECT_IN, "<");
	// t_token *t1_6 = create_token(TOKEN_FILE, "1");
	// t_token *t1_7 = create_token(TOKEN_ARGS, "-R");
	
	t_token *t2 = create_token(TOKEN_ARGS, "cat");
	// t_token *t3 = create_token(TOKEN_FILE, "1");
	t_token *t4 = create_token(TOKEN_ARGS, "cat");
	//t_token *t5 = create_token(TOKEN_FILE, "2");
	t_token *t6 = create_token(TOKEN_PIPE, "|");
	t_token *t7 = create_token(TOKEN_ARGS, "cat");
	t_token *t8 = create_token(TOKEN_ARGS, "-e");
	// t_token *t7_1 = create_token(TOKEN_APPEND, ">>");
	// t_token *t7_2 = create_token(TOKEN_FILE, "2");
	// t_token *t8 = create_token(TOKEN_PIPE, "|");
	// t_token *t9 = create_token(TOKEN_ARGS, "ls -a");
	// t_token *t10 = create_token(TOKEN_PIPE, "|");
	// t_token *t11 = create_token(TOKEN_ARGS, "cat -b -e");


	// t1->next = t1_2;
	// t1_2->prev = t1;
	// t1_2->next = t1_3;
	// t1_3->prev = t1_2;
	// t1_3->next = t1_4;
	// t1_4->prev = t1_3;
	// t1_4->next = t1_5;
	// t1_5->prev = t1_4;
	// t1_5->next = t1_6;
	// t1_6->prev = t1_5;
	// t1_6->next = t1_7;
	// t1_7->prev = t1_6;
	t1->next = t2;
	t2->prev = t1;

	t2->next = t4;
	t4->prev = t2;

	t4->next = t6;
	t6->prev = t4;

	t6->next = t7;
	t7->prev = t6;
	t7->next = t8;
	t8->prev = t7;
	// t5->next = t6;
	// t6->prev = t5;
	// t6->next = t7;
	// t7->prev = t6;
	// t7->next = t7_1;
	// t7_1->prev = t7;
	// t7_1->next = t7_2;
	// t7_2->prev = t7_1;
	
	// t7_2->next = t8;
	// t8->prev = t7_2;

	// t8->next = t9;
	// t9->prev = t8;
	// t9->next = t10;
	// t10->prev = t9;
	// t10->next = t11;
	// t11->prev = t10;
	t_gc	*gc = get_gc();
	// t_gc_list* gc_lst = init_gc_list();
	t_cmd_block *cmd_block_list = NULL;
	grouplize(t1, &cmd_block_list, gc->temp);
	int i = 0;
	t_cmd_block *check = cmd_block_list;
	
	// while (check)
	// {
	// 	while (check->args && check->args[i])
	// 	{
	// 		if (check->args && check->args[i])
	// 			printf("args : %s\n", check->args[i]);
	// 		i++;
	// 	}
	// 	while (check->io_streams && check->io_streams->infile_name)
	// 	{
	// 		if (check->io_streams->infile_name)
	// 			printf("file : %s\n", check->io_streams->infile_name);
	// 			check->io_streams = check->io_streams->next;
	// 	}
	// 	printf("cmd_block_list %p\n",check);
	// 	check = check->next;
	// }


	// if(cmd_block_list)
	// {
	// 	if (cmd_block_list->built_in)
	// 		printf("cmd_block_list->built_in : %s\n", cmd_block_list->built_in);
	// 	if (cmd_block_list->args)
	// 		printf("cmd_block_list->args : %s\n", cmd_block_list->args);
	// 	printf("cmd_block_list->io_streams->infile_name : %s\n", cmd_block_list->io_streams->infile_name);
	// 	// printf("cmd_block_list->io_streams->next->infile_name : %s\n", cmd_block_list->io_streams->next->infile_name);
	// 	// fprintf(stderr, "cmd->io_streams->heredoc_eof %s\n", cmd_block_list->io_streams->heredoc_eof);
	// 	// printf("cmd_block_list->io_streams->outfile_name : %s\n", cmd_block_list->io_streams->outfile_name);
	// 	// printf("cmd_block_list->io_streams->next->outfile_name :%s\n", cmd_block_list->io_streams->next->outfile_name);
	// 	// printf("cmd_block_list->next->io_streams->heredoc_eof : %s\n", cmd_block_list->next->io_streams->heredoc_eof);
	// 	// printf("cmd_block_list->pipe : %p\n", cmd_block_list->pipe);
	// 	printf("cmd_block_list->io_streams->append_filename : %s\n", cmd_block_list->io_streams->append_file_name);
	// }
	// if (cmd_block_list->next)
	// {
	// 	printf("cmd_block_list->next->cmd : %p, %s\n",cmd_block_list->next, cmd_block_list->next->args);
	// }
	// else
	// {
	// 	printf("no pipe\n");
	// }
	t_shell *shell = get_shell();
	shell ->my_envp = copy_envp(gc->shell, envp);
	main_execute(cmd_block_list, gc->temp, shell);
	printf(YELLOW"shell->last_status_exit : %d\n"DEFAULT, shell->last_status_exit);
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
