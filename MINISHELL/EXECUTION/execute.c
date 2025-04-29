# include "../INCLUDE/main.h"

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void	wait_for_child_and_update_status(t_cmd_block *cur, int i)
{
	t_shell *shell = get_shell();
	int status;
	int	idx;

	idx = 0;
	while(idx < i)
	{
		fprintf(stderr, RED"shell->pids[idx] %d\n"DEFAULT, shell->pids[idx]);
		pid_t child_pid = wait4(shell->pids[idx], &status, 0 ,NULL);
		fprintf(stderr ,RED"child_pid %d\n"DEFAULT, child_pid);
		fprintf(stderr ,BLUE"parent got this from wait4() child_pid : %d\n"DEFAULT, child_pid);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		else if (WIFSIGNALED(status))
		{
			fprintf(stderr, RED"terminated by signal %d (%s)\n" DEFAULT, WTERMSIG(status), strsignal(WTERMSIG(status)));
			shell->last_status_exit = 128 + WTERMSIG(status);
		}
		idx++;
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

void	execute_builtin(t_cmd_block *cur, t_shell *shell)
{
	t_gc *gc = get_gc();
 	
	fprintf(stderr, GREEN"execute_builtin()\n"DEFAULT);
 	if (strcmp(cur->built_in, "cd") == 0)
	{
		fprintf(stderr, RED"try to do 'cd' execute_builtin\n"DEFAULT);
		cd(cur->args, shell, gc);
	}
	if (strcmp(cur->built_in, "echo") == 0)
	{
		fprintf(stderr, RED"try to do 'echo' execute_builtin\n"DEFAULT);
		ft_echo(cur->args, shell);
	}
	// if (strcmp(cur->built_in, "export") == 0)
	// 	export(shell, "export");
	// if (strcmp(cur->built_in, "pwd") == 0)
	// 	my_pwd(shell, gc);
	// if (strcmp(cur->built_in, "env") == 0)
	// 	print_envp(shell, "env");
	
	// if (strcmp(cur->built_in, "unset") == 0)
	// 	unset(cur->args, shell);
}
	
void	single_cmd_execute(t_cmd_block *cur, pid_t pid, t_gc *gc)
{
	t_shell	*shell;

	if (!cur)
		return;
	fprintf(stderr, GREEN"single_cmd_execute()\n"DEFAULT);
	shell = get_shell();
	if (cur->built_in)
	{
		if (cur->io_streams && cur->io_streams->heredoc_eof)
		{
			if (heredoc_fd_offset_and_redir(cur) == -1)
			{
				perror("errror");
				gc_free(gc);
				exit(1);
			}
		}
		set_io_streams(cur);
		execute_builtin(cur, shell);
	}
	else if(cur->args) //if external cmd
	{
		fprintf(stderr, "is in single_cmd_execute for child??\n");
		pid = fork();
		shell->pids[0] = pid;
		fprintf(stderr, YELLOW" singlecmd fork() : %d , shell->pids[0] %d \n"DEFAULT, pid, shell->pids[0]) ;
		if (pid == 0)
		{
			if (cur->io_streams && cur->io_streams->heredoc_eof)
			{
				if (heredoc_fd_offset_and_redir(cur) == -1)
				{
					perror("errror");
					gc_free(gc);
					exit(1);
				}
			}
			set_io_streams(cur);
			run_execve(cur, gc);
		}
		else
		 	wait_for_child_and_update_status(cur, 1);
	}
}

void	execute_child(pid_t pid, t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	set_io_streams(cur);
	processing_pipe(cur, shell, gc->temp);
	if (cur->io_streams && cur->io_streams->heredoc_eof)
	{
		if (heredoc_fd_offset_and_redir(cur) == -1)
		{
			perror("errror");
			gc_free(gc);
			exit(1);
		}
	}
	if (cur->built_in)
	{
		execute_builtin(cur, shell);
		//exit(0);
	}
	if (cur->args)
	{
		run_execve(cur, gc);
	}
}

void 	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	char	**splitted_path;
	t_shell *shell;
	bool 	is_executable;

	is_executable = false;
	shell = get_shell();
	char *path = find_var_in_env(shell->my_envp, "PATH", 4, gc->temp);
	if (!path)
	{
		exit(1);
		gc_free(gc);
	}
	splitted_path = ft_split(path, ':');
	int i = 0;
	while(splitted_path[i])
	{
		char *attach_slash_to_cmd = ft_strjoin(splitted_path[i], "/");
		if (!attach_slash_to_cmd)
		{
			exit(1);
			gc_free(gc);
		}
		char *cmd_path = ft_strjoin(attach_slash_to_cmd, cmd_block->args[0]);
		if (!cmd_path)
		{
			exit(1);
			gc_free(gc);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			fprintf(stderr, "execve()\n");
			if (execve(cmd_path , cmd_block->args, shell->my_envp) == -1)
			{
				fprintf(stderr, RED"error\n"DEFAULT);
				exit(1);
			}
		}
		i++;
	}
	fprintf(stderr, RED"command not found: %s\n"DEFAULT, cmd_block->args[0]);
	exit(127);
}


void	main_execute(t_cmd_block *cmd_block, t_gc *gc, t_shell *shell)
{
	t_cmd_block *cur;
	pid_t	pid;

	//t_shell *shell = get_shell();
	cur = cmd_block;
	//validate_check(cur);
	
	hanlde_heredoc(cur);

	//memo zahlen wie viele pipe es gibt
	int	pid_counts = count_command(cmd_block);
	printf("pid_counts %d\n", pid_counts);

	do_alloc_pids(cmd_block);

	if (cur->is_built_in)
		execute_single_command(cur);
	
	execute_pipeline(cur);

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


int main(int ac, char *argv[], char *envp[])
{
	(void)ac;
	(void)argv;
	t_token *t1 = create_token(TOKEN_BUILT_IN, "cd");
	t_token *t2 = create_token(TOKEN_ARGS, "..");
	t_token *t3 = create_token(TOKEN_PIPE, "|");
	t_token *t4 = create_token(TOKEN_BUILT_IN, "echo");
	t_token *t5= create_token(TOKEN_ARGS, "hello");
	// t_token *t6 = create_token(TOKEN_ARGS, "-e");
	// t_token *t7 = create_token(TOKEN_APPEND, ">>");
	// t_token *t8 = create_token(TOKEN_FILE, "2");

	// t_token *t8 = create_token(TOKEN_PIPE, "|");
	// t_token *t9 = create_token(TOKEN_ARGS, "ls -a");
	// t_token *t10 = create_token(TOKEN_PIPE, "|");
	// t_token *t11 = create_token(TOKEN_ARGS, "cat -b -e");

	//t1->next = t2;
	t1->prev = NULL;
	t1->next = t2;

	t2->prev = t1;
	t2->next = t3;

	t3->prev = t2;
	t3->next = t4;

	t4->prev = t3;
	t4->next = t5;

	// t5->prev = t4;
	// t5->next = t6;

	// t6->prev = t5;
	// t6->next = t7;

	// t7->prev = t6;
	// t7->next = t8;

	// t8->prev = t7;
	// t8->next = NULL;

	// t8->next = t9;
	// t9->prev = t8;
	// t9->next = t10;
	// t10->prev = t9;
	// t10->next = t11;
	// t11->prev = t10;
	t_gc	*gc = get_gc();
	// t_gc_list* gc_lst = init_gc_list();
	t_cmd_block *cmd_block_list = NULL;
	grouplize(t1, &cmd_block_list, gc);
	int i = 0;
	t_cmd_block *print = cmd_block_list;
	
	while (print)
	{
		i = 0;
		while (print->args && print->args[i])
		{
			if (print->args && print->args[i])
				printf("args : %s\n", print->args[i]);
			i++;
		}
		t_io_streams_list *tmp = print->io_streams;
		while (tmp&& tmp->infile_name)
		{
			if (tmp->infile_name)
				printf("file : %s\n", tmp->infile_name);
				tmp = tmp->next;
		}
		tmp = print->io_streams;
		while (tmp && tmp->heredoc_eof)
		{
			if (tmp->heredoc_eof)
				printf("heredoc_eof : %s \n", tmp->heredoc_eof);
				tmp = tmp->next;
		}

		printf("cmd_block_list %p\n",print);
		print = print->next;
	}

	t_shell *shell = get_shell();
	shell->my_envp = copy_envp(gc->shell, envp);
	main_execute(cmd_block_list, gc, shell);
	printf(YELLOW"shell->last_status_exit : %d\n"DEFAULT, shell->last_status_exit);
	gc_free(gc);
	return 0;
}

//memo refactoringrefactoringrefactoringrefactoringrefactoringrefactoringrefactoringrefactoringrefactoringrefactoringrefactoring
void	validate_check(t_cmd_block *cmd_block)
{
	t_shell *shell;
	t_cmd_block *cur;
	t_gc *gc;

	gc = get_gc();
	shell =get_shell();
	cur = cmd_block;
	if (!cmd_block || is_validate_cmd_block(cur) == false)
	{
		gc_free(gc);
		perror(RED"non valid cmd"DEFAULT);
		shell->last_status_exit = 1;
		exit(1);
	}
}

void	hanlde_heredoc(t_cmd_block *cmd_block)
{
	t_cmd_block *cur = cmd_block;
	while(cur)
	{
		fprintf(stderr, RED"cur->io_streams %p\n"DEFAULT, cur->io_streams );
		if (cur->io_streams && cur->io_streams->heredoc_eof)
		{
			fprintf(stderr, RED"cur->io_streams->heredoc_eof %s\n"DEFAULT, cur->io_streams->heredoc_eof);
			process_heredoc(cur->io_streams);
			printf(RED"heredocfd %d\n"DEFAULT, cur->io_streams->heredoc_fd);
		}
		cur = cur->next;
	}
}

void	execute_single_command(t_cmd_block *cmd_block)
{
	pid_t pid;
	t_gc *gc;

	gc = get_gc();
	if(cmd_block && !cmd_block->prev && !cmd_block->next)
	{
		fprintf(stderr, RED"execute_single_command()\n"DEFAULT);
		single_cmd_execute(cmd_block, pid, gc);
	}
}

int	count_command(t_cmd_block *cmd_block)
{
	t_cmd_block *temp;
	temp = cmd_block;
	int count = 0;

	while(temp)
	{
		count++;
		temp = temp->next;
	}
	return count;
}


void	do_alloc_pids(t_cmd_block* cmd_block)
{
	int	count;
	t_shell *shell;
	t_gc	*gc;

	gc = get_gc();
	count = count_command(cmd_block);
	if (count == 0 || (count == 1 && cmd_block->built_in))
		return ;
	shell = get_shell();
	printf("count : %d\n", count);
	shell->pids = do_alloc(&gc->temp, sizeof(pid_t) * count, TYPE_SINGLE_PTR, "pids");
	if(!shell->pids)
	{
		gc_free(gc);
		exit(1);
	}
}

void	fork_and_execute(t_cmd_block *cmd_block, t_gc *gc, int *i)
{
	t_cmd_block *cur;
	pid_t pid;
	t_shell	*shell;

	shell = get_shell();
	cur = cmd_block;
	if (cur->next)
	{
		add_pipe(&cur, gc->temp);
		fprintf(stderr, YELLOW"[pid %d] p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT,getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
	}
	pid = fork();
	fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
	if (pid == 0)
		execute_child(pid, cur, gc, shell);
		
	close_pipefd(cur);
	shell->pids[*i] = pid;
}

//error
void	execute_pipeline(t_cmd_block *cmd_block)
{
	t_cmd_block *cur;
	t_gc 		*gc;
	int	i;

	i = 0;
	gc = get_gc();
	cur = cmd_block;

	while(cur)
	{
		fork_and_execute(cur, gc, &i);
		i++;
		cur = cur->next;
	}
	wait_for_child_and_update_status(cmd_block, i);
}