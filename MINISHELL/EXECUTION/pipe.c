# include "../INCLUDE/main.h"

//fstat : for getting infos of files. prob can use just to debug fd of pipe or redirection ?? to check pipe's permission or pipes buffer size its good!!!
//test!

//use fpirintf! stderr isn't not for error otherwise for printing out without bufffering.
void echo_Hello_pipe_cat_pipe_wc(t_shell *shell)
{
	char *args1[] = {"/usr/bin/cat", NULL};
	char *args2[] = {"/usr/bin/wc", NULL};

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
		dup2(fd[1], STDOUT_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd[1], STDOUT_FILENO);
		close(fd[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[1]);
		//find echo excute echo
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
	//waitpid(pid, &status, 0);

	int fd1[2];
	pipe(fd1);
	fprintf(stderr, YELLOW "pipe() [%d, %d]\n" DEFAULT, fd1[0], fd1[1]);
	pid	= fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //85644
	{
		dup2(fd[0], STDIN_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd[0], STDIN_FILENO);
		close(fd[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd[0]);
		close(fd1[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
		dup2(fd1[1], STDOUT_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[1], STDOUT_FILENO);
		close(fd1[1]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[1]);
		execve("/usr/bin/cat", args1, shell->my_envp);
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
	//waitpid(pid, status, 0);

	pid = fork();
	fprintf(stderr, YELLOW "fork() = %d\n" DEFAULT, pid);
	if (pid == 0) //85645
	{
		dup2(fd1[0], STDIN_FILENO);
		fprintf(stderr, YELLOW "[pid %d] dup2([%d, %d)\n" DEFAULT,getpid(), fd1[0], STDIN_FILENO);
		close(fd1[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
		execve("/usr/bin/wc", args2 , shell->my_envp);
		fprintf(stderr, YELLOW "[pid %d], execve wc \n" DEFAULT, getpid());
	}
	else
	{
		close(fd1[0]);
		fprintf(stderr, YELLOW "[pid %d], close[%d]\n" DEFAULT, getpid(), fd1[0]);
	}
	//to check 
	while (waitpid(-1, NULL, WNOHANG) > 0);
}