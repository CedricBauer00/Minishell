#include "execution.h"

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

void	add_pipe(t_cmd_block **cmd_block, t_gc_list *gc_lst)
{
	t_pipe *new_pipe_node;
	// t_pipe *lastnode;

	if (!cmd_block || !*cmd_block)
		return ;
	new_pipe_node = init_pipe(gc_lst);
	if ((*cmd_block)->pipe == NULL)
	{
		(*cmd_block)->pipe = new_pipe_node;
	}
	// if ((*command)->pipe == NULL)
	// {
	// 	(*command)->pipe = new_pipe_node;
	// 	new_pipe_node->prev = NULL;
	// }
	// else
	// {
	// 	lastnode = (*command)->pipe;
	// 	printf("lastnode ->next : %p\n",  lastnode->next);
    //     while (lastnode->next)
    //         lastnode = lastnode->next;
    //     lastnode->next = new_pipe_node;
    //     new_pipe_node->prev = lastnode;
	// }
	// new_pipe_node->next = NULL;
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

// bool	is_first_pipe_cmd(t_cmd_block *cmd)
// {
// 	if (cmd->next && !cmd->prev)
// 		return true;
// 	return false;
// }

// bool	is_middle_pipe_cmd(t_cmd_block *cmd) //prev->read_end->fd exist
// {
// 	if (cmd->prev && cmd->next)
// 		return true;
// 	return false;
// }

// bool	is_last_pipe_cmd(t_cmd_block *cmd) // prev->read_end->fd exist
// {
// 	if(cmd->prev && !cmd->next)
// 		return true;
// 	return false;
// }

// bool	is_single_pipe(t_command *command) //in disenfall muss ich nur first and last aufrufen.
// {
// 	return (is_first_pipe(command) && is_last_pipe(command));
// }

// bool	is_multiple_pipe(t_command *command) //in diesenfall muss ich alle aufrufen.
// {
// 	return (is_middle_pipe(command));
// }

bool	is_first_pipe(t_cmd_block *cmd)
{
	if (!cmd->prev && cmd->next)
		return true;
	else
		return false;
}

bool	is_middle_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && cmd->next)
		return true;
	else
		return false;
}

bool	is_last_pipe(t_cmd_block *cmd)
{
	if (cmd->prev && !cmd->next)
		return true;
	else
		return false;
}

int	first_pipe_cmd(t_cmd_block *command, t_shell *shell, t_gc_list *gc_lst)
{
	// if (pid == 0)
	// {
		//handle_re_dir(command);
		fprintf(stderr,BLUE"first_pipe getpid() %d\n"DEFAULT, getpid());
		close(command->pipe->pipefd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[0]);
		if (dup2( command->pipe->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror(RED"first cmd dup2 error\n"DEFAULT);
			return (-1);
		}
		fprintf(stderr, YELLOW "FIRST PIPE[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->pipefd[1], STDOUT_FILENO);
		close( command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(),  command->pipe->pipefd[1]);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	//}
	// else
	// {
		// command->pipe->prev_read_end_fd = command->pipe->pipefd[0];
		// command->pipe->cur_fd_write_end = command->pipe->pipefd[1];
		// close(command->pipe->pipefd[1]);
		// fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(),  command->pipe->pipefd[1]);
	// }
	return (1);
}

//memo if its multiple pipe lines...
int	middle_pipe_cmd(t_cmd_block *command, t_shell *shell, t_gc_list *gc_lst)
{
	// if (pid == 0)
	// {
		//handle_re_dir(command);
		fprintf(stderr,BLUE"second_pipe\n"DEFAULT);
		if (dup2(command->prev_read_end_fd , STDIN_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->prev_read_end_fd , STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(command->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->prev_read_end_fd );
		close(command->pipe->pipefd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[0]);
		if (dup2(command->pipe->pipefd[1], STDOUT_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->pipe->pipefd[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(command->pipe->pipefd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[1]);
	// }
	// else
	// {
		// close(command->pipe->prev_read_end_fd);
		// fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->prev_read_end_fd);
		// command->pipe->prev_read_end_fd = command->pipe->pipefd[0];
		// command->pipe->cur_fd_write_end = command->pipe->pipefd[1];
		// close(command->pipe->pipefd[1]);
		// fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->pipefd[1]);
	// }
	return 1;
}

int	last_pipe_cmd(t_cmd_block *command, t_shell *shell)
{
	// if (pid == 0)
	// {
		fprintf(stderr,BLUE"last_pipe getpid() %d\n"DEFAULT, getpid());
		//todo if theres re_dir_out then i need to call re_dir_out()
		
		if (dup2(command->prev_read_end_fd, STDIN_FILENO) == -1)
		{
			fprintf(stderr,RED"last_pipe_cmd() dup2()error\n"DEFAULT);
		}
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), command->prev_read_end_fd, STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(command->prev_read_end_fd);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->prev_read_end_fd);
	// }
	// else
	// {
		// close(command->pipe->prev_read_end_fd);
		// fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), command->pipe->prev_read_end_fd);
		// fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	//}
	return (1);
}

void	close_first_pipefd(t_cmd_block *cmd)
{
	fprintf(stderr,GREEN"in close_first_pipefd\n");
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	fprintf(stderr,GREEN"[pid %d] in close_first_pipefd, cmd->prev_read_end_fd : %d, cmd->cur_fd_write_end :%d\n"DEFAULT, getpid(),
		cmd->prev_read_end_fd, cmd->cur_fd_write_end);
	close(cmd->pipe->pipefd[1]);
	fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(),  cmd->pipe->pipefd[1]);
}

void	close_middle_pipefd(t_cmd_block *cmd)
{
	fprintf(stderr,GREEN"in close_middle_pipefd\n");
	close(cmd->prev_read_end_fd);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), cmd->prev_read_end_fd);
	cmd->next->prev_read_end_fd = cmd->pipe->pipefd[0];
	cmd->next->cur_fd_write_end = cmd->pipe->pipefd[1];
	close(cmd->pipe->pipefd[1]);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), cmd->pipe->pipefd[1]);
}

void	close_last_pipefd(t_cmd_block *cmd)
{
	fprintf(stderr,GREEN"in close_last_pipefd()\n");
	close(cmd->prev->prev_read_end_fd);
	fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), cmd->prev_read_end_fd);
	fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
}

void	close_pipefd(t_cmd_block *cmd)
{
	if (!cmd->prev && cmd->next)
		close_first_pipefd(cmd);
	else if (cmd->prev && cmd->next)
		close_middle_pipefd(cmd);
	else if (cmd->prev && !cmd->next)
		close_last_pipefd(cmd);
}
//memo if return -1 we have to free all allocated memories
void	processing_pipe(t_cmd_block *cmd, t_shell *shell, t_gc_list* gc_lst)
{
	if (is_first_pipe(cmd))
		first_pipe_cmd(cmd, shell, gc_lst);
	else if (is_middle_pipe(cmd))
		middle_pipe_cmd(cmd, shell, gc_lst);
	else if (is_last_pipe(cmd))
		last_pipe_cmd(cmd, shell);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	t_gc_list *gc_lst = init_gc_list();
// 	t_shell *shell = get_shell();

// 	//todo copy_envp should put into init_shell func.
// 	shell->my_envp = copy_envp(gc_lst, envp);
	
// 	// export(argv, shell);
// 	// printf("argv[1]%s, argv[2]%s\n", argv[1],argv[2]);
// 	// unset(argv, shell);
	
// 	// ft_echo(argv, shell);

// 	//test
// 	// int i = 0;
// 	// while (shell->my_envp[i])
// 	// {
// 	// 	printf(BLUE"%p,  %s\n"DEFAULT, shell->my_envp[i], shell->my_envp[i]);
// 	// 	i++;
// 	// }
// 	//echo_Hello_pipe_cat_pipe_wc(shell);
// 	char *args1[] = {"echo","hello", NULL};
// 	t_token *command1 = add_token(gc_lst, args1[0], args1, TOKEN_CMD);

// 	t_token *command2 = add_token(gc_lst, NULL, NULL, TOKEN_PIPE);

// 	char *args2[] = {"cat" , NULL};
// 	t_token *command3 = add_token(gc_lst, args2[0], args2, TOKEN_CMD);

// 	t_token *command4 = add_token(gc_lst, NULL, NULL, TOKEN_PIPE);

// 	char *args3[] = {"wc" , NULL};
// 	t_token *command5 = add_token(gc_lst, args3[0], args3, TOKEN_CMD);

// 	command1->next = command2;
// 	command2->prev = command1;
// 	command2->next =command3;
// 	command3->prev = command2;
// 	command3->next = command4;
// 	command4->prev = command3;
// 	command4->next = command5;
// 	command5->prev = command4;
// 	command5->next = NULL;

// 	first_pipe_cmd(command1,shell,gc_lst);
// 	middle_pipe_cmd(command2,shell,gc_lst);
// 	last_pipe_cmd(command3,shell);
// }

