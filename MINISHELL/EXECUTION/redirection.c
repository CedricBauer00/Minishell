# include "../INCLUDE/main.h"

//todo both redirection must be called in child!
// todo need to change if 

int	re_dir_in(t_token *token)
{
	t_token *cur;
	int		fd;

	cur = token;
	fd = open(cur->next->infile_name, O_RDONLY);
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
	return 1;
}

//todo need to change if 
int	re_dir_out(t_token *token)
{
	t_token *cur;
	int		fd;

	cur = token;
	fd = open(cur->next->outfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	return 1;
}

int	handle_re_dir(t_token *token)
{
	if(token->type & TOKEN_REDIRECT_IN)
		re_dir_in(token);
	if (token->type & (TOKEN_REDIRECT_OUT))
		re_dir_out(token);
	return 0;
}


