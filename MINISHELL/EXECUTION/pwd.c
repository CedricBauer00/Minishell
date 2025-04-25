#include "../INCLUDE/main.h"

//pwd getcwd() get current working directory
//obwohl ist es NULL, muss die richtige weg return --> dann brauche ich hier die find_var_in_env funktion nicht zu verwenden.

// char	*get_cwd(t_shell *shell, t_gc_list *gc_lst)
// {
// 	char	*cwd;

// 	char *cwd = find_var_in_env(shell->my_envp, "PWD", 3, gc_lst);
// 	if (cwd == NULL)
// 	{
// 		char *temp = getcwd(NULL, 0);
// 		cwd = (char*)do_alloc(gc_lst, ft_strlen(temp) + 1, TYPE_SINGLE_PTR, "getcwd");
// 		if (!cwd)
// 		{
// 			free(temp);
// 			return NULL;
// 		}
// 		ft_strlcpy(cwd, temp, ft_strlen(temp) + 1);
// 		free(temp);
// 		return cwd;
// 	}
// 	return cwd;
// }

char	*my_getcwd(t_shell *shell, t_gc *gc)
{
	char	*cwd;
	char	*temp;

	temp = getcwd(NULL, 0);
	if (!temp)
	{
		perror(RED"getcwd()failed"DEFAULT);
		//todo allfree
		exit(1);
	}
	cwd = (char*)do_alloc(gc->shell, ft_strlen(temp) + 1, TYPE_SINGLE_PTR, "getcwd");
	if (!cwd)
	{
		free(temp);
		return NULL;
	}
	ft_strlcpy(cwd, temp, ft_strlen(temp) + 1);
	free(temp);
	return cwd;
}

void	my_pwd(t_shell *shell, t_gc_list *gc_lst)
{
	char *pwd;
	pwd = my_getcwd(shell, gc_lst);
	printf(GREEN"%s\n"DEFAULT, pwd);
	//todo
	// t_gc_list *find = find_node();
	// delete_node(find);
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