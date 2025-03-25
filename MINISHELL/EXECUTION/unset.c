# include "../INCLUDE/main.h"

/*
	unset delte path in env
	but couldn't delte readonly file.
	and after getting rid of "PATH" then find anymore env.

*/

//S_IWUSR	: write permmision for USER
//S_IWGRP	: write permission for GROUP
//S_IWOTH	: write permission for OTHERS
//this way just only for checking system vars.

// int	is_readonly(const char *path)
// {
// 	struct stat file_stat;

// 	if (stat(path, &file_stat) != 0)
// 	{
// 		perror("failed stat()");
// 		return 0;
// 	}
// 	if (!(file_stat.st_mode & (S_IWUSR | S_IWGRP | S_IWOTH)))
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

//todo think about multiple env NAME should work like "unset PWD OLDPWD HOME PATH"
void	unset(char **argv, t_shell *shell)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	if ((strcmp("unset", argv[1]) == 0) && argv[2] != NULL)
	{
		i = 2;
		while (argv[i])
		{
			found = -1;
			j = 0;
			while (shell->my_envp[j])
			{
				if ((strncmp(shell->my_envp[j], argv[i], ft_strlen(argv[i])) == 0) && (shell->my_envp[j][strlen(argv[i])] == '='))
				{
					found = j;
					break;
				}
				j++;
			}
			printf(GREEN"%d\n"DEFAULT, found);
			if (found != -1)
			{
				free(shell->my_envp[found]);
				while(shell->my_envp[found + 1])
				{
					shell->my_envp[found] = shell->my_envp[found + 1];
					found++;
				}
				shell->my_envp[found] = NULL;
			}
			i++;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_gc_list *head = init_gc_list();
	t_shell *shell = get_shell();

	//todo copy_envp should put into init_shell func.
	shell->my_envp = copy_envp(head, envp);

	export(argv, shell);
	printf("argv[1]%s, argv[2]%s\n", argv[1],argv[2]);
	unset(argv, shell);
	
	ft_echo(argv, shell);
	//test
	// int i = 0;
	// while (shell->my_envp[i])
	// {
	// 	printf(BLUE"%p,  %s\n"DEFAULT, shell->my_envp[i], shell->my_envp[i]);
	// 	i++;
	// }
}
