# include "../INCLUDE/main.h"

//fstat : for getting infos of files. prob can use just to debug fd of pipe or redirection ?? to check pipe's permission or pipes buffer size its good!!!
//test!

//use fpirintf! stderr isn't not for error otherwise for printing out without bufffering.
void echo_Hello_pipe_cat_pipe_wc(t_shell *shell)
{
	char *args1[] = {"cat", NULL};
	char *args2[] = {"wc", "-l", NULL};

	int fd[2];
	pid_t pid;
	int status;

	pipe(fd);
	fprintf(stderr, YELLOW "pipe() [%d, %d]\n" DEFAULT, fd[0], fd[1]);
	pid = fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //80460
	{
		close(fd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) ==-1)
			perror(RED"first cmd dup2 error\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(fd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[1]);
		write(1, "hello\n", 6);
		fprintf(stderr, YELLOW"[pid %d] write(1, hello, 6)\n"DEFAULT, getpid());
		exit(0);
		fprintf(stderr, YELLOW "exited with 0\n" DEFAULT);
	}
	//parent 85642
	else
	{
		close(fd[1]);
		fprintf(stderr, YELLOW "[%d], close[%d]\n" DEFAULT, getpid(), fd[1]);
		// dup2(fd[0], STDIN_FILENO);
		// close(fd[0]);
	}

	int fd1[2];
	if (pipe(fd1) == -1)
		perror(RED"SEC PIPE ERROR\n"DEFAULT);
	fprintf(stderr, YELLOW "pipe() [%d, %d]\n" DEFAULT, fd1[0], fd1[1]);
	pid	= fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //85644
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd[0], STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(fd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
		close(fd1[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
		if (dup2(fd1[1], STDOUT_FILENO) == -1)
			perror(RED"SE dup2 ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[1], STDOUT_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(fd1[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[1]);
		if(execve("/bin/cat", args1, shell->my_envp) == -1)
			perror(RED"cat failed\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d], execve cat \n" DEFAULT, getpid());
		exit(0);
	}
	else
	{
		close(fd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
		close(fd1[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[1]);
	}

	pid = fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //85645
	{
		dup2(fd1[0], STDIN_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[0], STDIN_FILENO);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
		close(fd1[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
		//write(1,"hello\n",6);
		if (execve("/usr/bin/wc", args2 , shell->my_envp) ==-1)
			perror(RED"WC ERROR\n"DEFAULT);
		fprintf(stderr, YELLOW "[pid %d], execve wc \n" DEFAULT, getpid());
		exit(0);
	}
	else
	{
		close(fd1[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
		fprintf(stderr, "STDIN_FILENO: %d, STDOUT_FILENO: %d\n", STDIN_FILENO, STDOUT_FILENO);
	}
	//to check 
	while ((pid = wait(&status)) > 0) 
	{
		fprintf(stderr, "Process %d exited with status %d\n", pid, WEXITSTATUS(status));
	}
}

/*
	1.파이프가 여러개일경우
	if(첫번째 노드인경우, 앞에 아무것도 없고 뒤에 머가있는경우)
		if (dup2(fd[1], STDOUT_FILENO) ==-1)
			perror(RED"first cmd dup2 error\n"DEFAULT);
	if(마지막 노드인 경우, 앞에 뭐가있고,? 뒤에 머가 없는경우)
		dup2(fd1[0], STDIN_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[0], STDIN_FILENO);

*/