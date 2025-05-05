#include "execution.h"

char	*my_getcwd()
{
	char	*cwd;
	char	*temp;
	t_gc	*gc;

	gc = get_gc();
	temp = getcwd(NULL, 0);
	if (!temp)
	{
		perror(RED"getcwd()failed"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	cwd = (char*)do_alloc(&gc->temp, ft_strlen(temp) + 1, TYPE_SINGLE_PTR, "getcwd");
	if (!cwd)
	{
		gc_free(gc);
		return NULL; 
	}
	ft_strlcpy(cwd, temp, ft_strlen(temp) + 1);
	free(temp);
	return cwd;
}

void	ft_pwd(char **args, t_gc *gc)
{
	t_gc_list	*find;

	find = NULL;
	if (*args)
	{
		printf(RED"PWD TOO MANY ARGS\n"DEFAULT);
		//all_free(&gc->temp);
		return ;
	}
	char *pwd;
	pwd = my_getcwd();
	printf(YELLOW"%s\n"DEFAULT, pwd);
	find = find_node(find, (char*)pwd);
	delete_node(&gc->temp, (t_gc_list*)find);
}

// void	pwd(t_shell *shell, t_gc_list *gc_lst)
// {
// 	get_cwd();
// }

// int main()
// {
// 	char *cwd = pwd();
// 	if (cwd)
// 	{
// 		printf("%p, %s\n", cwd, cwd);
// 		free(cwd);
// 		cwd = NULL;
// 		if (cwd == NULL)
// 			printf("\033[1;34mcwd is already free\033[0;34m\n");
// 	}
// }