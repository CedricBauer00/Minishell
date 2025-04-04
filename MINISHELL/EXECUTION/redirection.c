# include "../INCLUDE/main.h"

//todo both redirection must be called in child!

/*
	if(cur ->prev && cur->prev->type->TOKEN_REDIRECT_IN)
	{
		pid_t;

		pid = fork();
		if (pid == 0)
		{
			re_dir_in(token);
			execve(cmd);
		}
		else
		{

		}
	}
	
	
*/
 // todo need to change if 
int	re_dir_in(t_token *token)
{
	t_token *cur;
	int		fd;

	cur = token;
	if(cur->type == TOKEN_REDIRECT_IN)
	{
		fd = open(cur->next->value, O_RDONLY);
		if (fd == -1)
		{
			perror(RED"re_dir_in faield\n"DEFAULT);
			return -1;
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror(RED"re_dir_in() dup2 error\n"DEFAULT);
			close(fd);
			return -1;
		}
		close(fd);
	}
	return 1;
}

/*
	if (cur ->next && cur->next->type->TOKEN_REDIRECT_OUT)
	{
		pid_t;

		pid = fork();
		if (pid == 0)
		{
			re_dir_out(token);
			execve(cmd);
		}
		else
		{
			close(pid);
			//todo if next node is pipe?
		}
	}
*/

/*
	< filename cmd 
	cmd < filename
*/
//todo need to change if 
int	re_dir_out(t_token *token)
{
	t_token *cur;
	int		fd;

	cur = token;
	if(cur->type == TOKEN_REDIRECT_OUT)
	{
		fd = open(cur->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(RED"re_dir_in faield\n"DEFAULT);
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror(RED"re_dir_in() dup2 error\n"DEFAULT);
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return 1;
}


