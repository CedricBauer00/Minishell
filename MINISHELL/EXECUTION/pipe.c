# include "../INCLUDE/main.h"

//fstat : for getting infos of files. prob can use just to debug fd of pipe or redirection ?? to check pipe's permission or pipes buffer size its good!!!
//test!

//use fpirintf! stderr isn't not for error otherwise for printing out without bufffering.
// void echo_Hello_pipe_cat_pipe_wc(t_shell *shell)
// {
// 	char *args1[] = {"cat", NULL};
// 	char *args2[] = {"wc", "-l", NULL};

// 	int fd[2];
// 	pid_t pid;
// 	int status;

// 	pipe(fd);
// 	fprintf(stderr, YELLOW "pipe() [%d, %d]\n" DEFAULT, fd[0], fd[1]);
// 	pid = fork();
// 	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
// 	if (pid == 0) //80460
// 	{
// 		close(fd[0]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
// 		if (dup2(fd[1], STDOUT_FILENO) ==-1)
// 			perror(RED"first cmd dup2 error\n"DEFAULT);
// 		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd[1], STDOUT_FILENO);
// 		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
// 		close(fd[1]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[1]);
// 		write(1, "hello\n", 6);
// 		fprintf(stderr, YELLOW"[pid %d] write(1, hello, 6)\n"DEFAULT, getpid());
// 		exit(0);
// 		fprintf(stderr, YELLOW "exited with 0\n" DEFAULT);
// 	}
// 	//parent 85642
// 	else
// 	{
// 		close(fd[1]);
// 		fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(), fd[1]);
// 	}

// 	int fd1[2];
// 	if (pipe(fd1) == -1)
// 		perror(RED"SEC PIPE ERROR\n"DEFAULT);
// 	fprintf(stderr, YELLOW "pipe() [%d, %d]\n" DEFAULT, fd1[0], fd1[1]);
// 	pid	= fork();
// 	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
// 	if (pid == 0) //85644
// 	{
// 		if (dup2(fd[0], STDIN_FILENO) == -1)
// 			perror(RED"SE dup2 ERROR\n"DEFAULT);
// 		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd[0], STDIN_FILENO);
// 		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
// 		close(fd[0]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
// 		close(fd1[0]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
// 		if (dup2(fd1[1], STDOUT_FILENO) == -1)
// 			perror(RED"SE dup2 ERROR\n"DEFAULT);
// 		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[1], STDOUT_FILENO);
// 		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
// 		close(fd1[1]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[1]);
// 		if (execve("/bin/cat", args1, shell->my_envp) == -1)
// 			perror(RED"cat failed\n"DEFAULT);
// 		fprintf(stderr, YELLOW "[pid %d], execve cat \n" DEFAULT, getpid());
// 		exit(0);
// 	}
// 	else
// 	{
// 		close(fd[0]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
// 		close(fd1[1]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[1]);
// 	}

// 	pid = fork();
// 	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
// 	if (pid == 0) //85645
// 	{
// 		dup2(fd1[0], STDIN_FILENO);
// 		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[0], STDIN_FILENO);
// 		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
// 		close(fd1[0]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
// 		//write(1,"hello\n",6);
// 		if (execve("/usr/bin/wc", args2 , shell->my_envp) ==-1)
// 			perror(RED"WC ERROR\n"DEFAULT);
// 		fprintf(stderr, YELLOW "[pid %d], execve wc \n" DEFAULT, getpid());
// 		exit(0);
// 	}
// 	else
// 	{
// 		close(fd1[0]);
// 		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
// 		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
// 	}
// 	//to check 
// 	while ((pid = wait(&status)) > 0) 
// 	{
// 		fprintf(stderr, "Process %d exited with status %d\n", pid, WEXITSTATUS(status));
// 	}
// }

//이리스트를 어떻게 release 할까?
//릴리즈 할시 가비지 컬렉터에서 헤드의 주소를 찾고 릴리즈~!
/*
	need to //test!
	add_pipe를 언제 호출해야하는지 생각해보기 -> in exe function.
*/

void	add_pipe(t_command **command, t_gc_list *gc_lst)
{
	t_pipe *new_pipe_node;
	t_pipe *lastnode;

	new_pipe_node = do_alloc(gc_lst, sizeof(t_pipe), TYPE_SINGLE_PTR);
		is_exited(new_pipe_node,gc_lst);
	new_pipe_node->pipefd = do_alloc(gc_lst, sizeof(int) * 2, TYPE_SINGLE_PTR);
		is_exited(new_pipe_node->pipefd,gc_lst);
	if (pipe(new_pipe_node->pipefd) == -1) 
	{
        perror("Error creating pipe");
        all_free(gc_lst);
        exit(EXIT_FAILURE);
    }
	if (!(*command)->pipe)
		(*command)->pipe = init_pipe_list(gc_lst); //todo Ich muss darüber nachdenken, wo ich die Funktion einsetzen soll. 
	if ((*command)->pipe == NULL)
	{
		(*command)->pipe = new_pipe_node;
		new_pipe_node->prev = NULL;
	}
	else
	{
		lastnode = (*command)->pipe;
        while (lastnode)
            lastnode = lastnode->next;
        lastnode->next = new_pipe_node;
        new_pipe_node->prev = lastnode;
	}
	new_pipe_node->next = NULL;
}

// int	create_pipe(t_token **token, t_gc_list *gc_lst)
// {
// 	t_token *cur;
// 	int		pipe_count;

// 	pipe_count = 0;
// 	cur = *token;
// 	while(cur)
// 	{
// 		if (cur->next && cur->next->type == TOKEN_PIPE)//todo fix if condition
// 		{
// 			add_pipe(cur, gc_lst);
// 			printf(YELLOW"created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
// 				cur->pipe_head->pipefd[0], cur->pipe_head->pipefd[1]);
// 			pipe_count++;
// 		}
// 		cur = cur->next;
// 	}
// 	return (pipe_count);
// }

bool	is_first_pipe_cmd(int *pipefd)
{
	if (!pipefd)
		return true;
	return false;
}

bool	is_middle_pipe_cmd(int fd_prev_read_end, int cur_fd_write_end) //prev->read_end->fd exist
{
	if (fd_prev_read_end != -1 && cur_fd_write_end != -1)
		return true;
	return false;
}

bool	is_last_pipe_cmd(int fd_prev_read_end, int cur_fd_write_end) // prev->read_end->fd exist
{
	if (fd_prev_read_end != -1 && cur_fd_write_end == -1)
		return true;
	return false;
}

bool	is_single_pipe(t_command *command) //in disenfall muss ich nur first and last aufrufen.
{
	return (is_first_pipe(command) && is_last_pipe(command));
}

bool	is_multiple_pipe(t_command *command) //in diesenfall muss ich alle aufrufen.
{
	return (is_middle_pipe(command));
}

int	first_pipe_cmd(t_command *command)
{
	pid_t	pid;

	pid = fork();

	//todo i think this if statment also could be in exe function later. not here.
	if (command->next && command->next->type == TOKEN_PIPE && is_first_pipe_cmd(command->pipe->pipefd))
    {
        add_pipe(&command, gc_lst);
        fprintf(YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
            command->pipe->pipefd[0], command->pipe->pipefd[1]);
    }
	if (pid == 0)
	{
		if (command->infile_name && command->type == TOKEN_REDIRECT_IN) //todo think about this case.
		{
			re_dir_in(command);
		}
		//todo if theres redirection i need to call re_dir_in
		close(command->pipe->pipefd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[0]);
		if (dup2( command->pipe->pipefd[1], STDOUT_FILENO) == -1)
			perror(RED"first cmd dup2 error\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->pipefd[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close( command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(),  command->pipe->pipefd[1]);
		//todo execve();
		exit(0);
		fprintf(stderr, YELLOW "exited with 0\n" DEFAULT);
	}
	else
	{
		command->pipe->prev_read_end_fd = command->pipe->pipefd[0];
		command->pipe->cur_fd_write_end = command->pipe->pipefd[1];
		close(command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(),  command->pipe->pipefd[1]);
	}
	return (1);
}

//memo if its multiple pipe lines...
int	middle_pipe_cmd(t_command *command, t_shell *shell)
{
	pid_t	pid;

	//todo i think this if statment also could be in exe function later. not here.
	if (command->next && command->next->type == TOKEN_PIPE && is_middle_pipe_cmd(command->pipe->prev_read_end_fd,command->pipe->cur_fd_write_end))
    {
        add_pipe(&command, gc_lst);
        fprintf(YELLOW"Created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
            command->pipe->pipefd[0], command->pipe->pipefd[1]);
    }
	pid = fork();
	//todo if cur && cur ->next ->type  == TOKENTYPEPIPE && cur->next->next
	if (pid == 0)
	{
		if (dup2(command->pipe->prev_read_end_fd , STDIN_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->prev_read_end_fd , STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(command->pipe->prev_read_end_fd );
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->prev_read_end_fd );
		close(command->pipe->pipefd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[0]);
		if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->pipefd[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[1]);
		if (execve("/bin/cat", args, shell->my_envp) == -1)
			perror(RED"cat failed\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d], execve cat \n" DEFAULT, getpid());
		exit(0);
	}
	else
	{
		close(command->pipe->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->prev_read_end_fd);
		command->pipe->prev_read_end_fd = command->pipe->pipefd[0];
		command->pipe->cur_fd_write_end = command->pipe->pipefd[1];
		close(command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[1]);
	}
	return 1;
}

int	last_pipe_cmd(t_command *command, t_shell *shell)
{
	pid_t	pid;

	//todo i think this if statment also could be in exe function later. not here.
	if (is_last_pipe_cmd(command->pipe->prev_read_end_fd, command->pipe->cur_fd_write_end))
	{
		printf(YELLOW"last_pipe_cd\n"DEFAULT);

	}
	pid = fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //todo fix if condition
	{
		//todo if theres re_dir_out then i need to call re_dir_out()
		dup2(command->pipe->prev_read_end_fd, STDIN_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->prev_read_end_fd, STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(command->pipe->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->prev_read_end_fd);
		if (execve("/usr/bin/wc", args , shell->my_envp) ==-1)
			perror(RED"WC ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d], execve wc \n" DEFAULT, getpid());
		exit(0);
	}
	else
	{
		close(command->pipe->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->prev_read_end_fd);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	}
	return (1);
}
