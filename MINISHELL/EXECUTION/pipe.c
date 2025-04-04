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
	//todo 
	need to //test!
	add_pipe를 언제 호출해야하는지 생각해보기,
	need to think when should i call first_pipe, middle_pipe, and last_pipe...

*/
void	add_pipe(t_token **token, t_gc_list *gc_lst)
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
	if (!(*token)->pipe_head && !(*token)->pipe_tail)
		init_pipe_list(); //todo Ich muss darüber nachdenken, wo ich die Funktion einsetzen soll.
	if ((*token)->pipe_head->next == NULL)
	{
		(*token)->pipe_head->next = new_pipe_node;
		new_pipe_node->next = (*token)->pipe_tail;
		new_pipe_node->prev = (*token)->pipe_head;
	}
	else
	{
		new_pipe_node->prev = (*token)->pipe_tail->prev;
		(*token)->pipe_tail->prev->next = new_pipe_node;
		(*token)->pipe_tail->prev = new_pipe_node;
		new_pipe_node->next = (*token)->pipe_tail;
	}
}

int	create_pipe(t_token **token, t_gc_list *gc_lst)
{
	t_token *cur;
	int		pipe_count;

	pipe_count = 0;
	cur = *token;
	while(cur)
	{
		if (cur->next && cur->next->type == TOKEN_PIPE)//todo fix if condition
		{
			add_pipe(cur, gc_lst);
			printf(YELLOW"created pipe_fd: read_end=%d, write_end=%d\n"DEFAULT,
				cur->pipe_head->pipefd[0], cur->pipe_head->pipefd[1]);
			pipe_count++;
		}
		cur = cur->next;
	}
	return (pipe_count);
}

bool	is_first_pipe(t_token *token)
{
	t_token *cur = token;
	while (cur != cur->pipe_head)
	{
		if (cur->prev && cur->prev->type == TOKEN_PIPE)
		{
			return false;
		}
		cur = cur->prev;
	}
	return true;
}

bool	is_last_pipe(t_token *token)
{
	t_token *cur = token;
	while(cur != cur->pipe_tail)
	{
		if(cur->next && cur->next->type == TOKEN_PIPE)
		{
			return false;
		}
		cur = cur->next;
	}
	return true;
}

bool	is_middle_pipe(t_token *token)
{
	return(!token->is_first_pipe && !token->is_last_pipe);
}

void	setup_pipe_flags(t_token *token)
{
	t_token *cur;

	cur = token;
	while(cur)
	{
		cur->is_first_pipe = is_first_pipe(cur);
		cur->is_last_pipe = is_last_pipe(cur);
		cur->is_middle_pipe = is_middle_pipe(cur);
		cur = cur->next;
	}
}

int	first_pipe_cmd(t_token *token)
{
	pid_t	pid;

	pid = fork();

	if (pid == 0)
	{
		close(token->pipe_head->pipefd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->pipefd[0]);
		if (dup2( token->pipe_head->pipefd[1], STDOUT_FILENO) == -1)
			perror(RED"first cmd dup2 error\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), token->pipe_head->pipefd[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close( token->pipe_head->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(),  token->pipe_head->pipefd[1]);
		//todo execve();
		exit(0);
		fprintf(stderr, YELLOW "exited with 0\n" DEFAULT);
	}
	else
	{
		close(token->pipe_head->pipefd[1]);
		fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(),  token->pipe_head->pipefd[1]);
	}
	token->pipe_head->prev_read_end_fd = token->pipe_head->pipefd[0];
	return (1);
}

//memo if its multiple pipe lines...
int	middle_pipe_cmd(t_token *token, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	//todo if cur && cur ->next ->type  == TOKENTYPEPIPE && cur->next->next
	if (pid == 0)
	{
		if (dup2(token->pipe_head->prev_read_end_fd , STDIN_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), token->pipe_head->prev_read_end_fd , STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(token->pipe_head->prev_read_end_fd );
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->prev_read_end_fd );
		close(token->pipe_head->pipefd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->pipefd[0]);
		if (dup2(token->pipe_head->pipefd[1], STDOUT_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), token->pipe_head->pipefd[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(token->pipe_head->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->pipefd[1]);
		if (execve("/bin/cat", args, shell->my_envp) == -1)
			perror(RED"cat failed\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d], execve cat \n" DEFAULT, getpid());
		exit(0);
	}
	else
	{
		close(token->pipe_head->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->prev_read_end_fd);
		close(token->pipe_head->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->pipefd[1]);
	}
	token->pipe_head->prev_read_end_fd = token->pipe_head->pipefd[0];
	return 1;
}

int	last_pipe_cmd(t_token *token, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //todo fix if condition
	{
		dup2(token->pipe_head->prev_read_end_fd, STDIN_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), token->pipe_head->prev_read_end_fd, STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(token->pipe_head->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->prev_read_end_fd);
		if (execve("/usr/bin/wc", args , shell->my_envp) ==-1)
			perror(RED"WC ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d], execve wc \n" DEFAULT, getpid());
		exit(0);
	}
	else
	{
		close(token->pipe_head->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), token->pipe_head->prev_read_end_fd);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	}
	return (1);
}
