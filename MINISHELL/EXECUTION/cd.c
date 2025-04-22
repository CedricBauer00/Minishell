/*
	cd[path] : move to path;
	cd or cd ~ : move to home;
	cd -: move to prev dir;
	cd ..: move to parent dir;
	cd /: move to root dir;
*/

/*
	//todo think!
	//how to free structure... using with garbage_colloector? or should i handle it separatly?
*/
#include "../INCLUDE/main.h"

//stat(): check path, and check if its approchable. succesed return 0, failed: return -1
//S_ISDIR is a macro 
//S_ISFIFO macro to check is pipe?
//S_IFREG macro to check is normal file?
//S_IFLNK	simbolick link

//ok!
int	is_valid_dir(const char *path)
{
	struct stat file_stat;

	if (stat(path, &file_stat) != 0)
	{
		perror("failed stat()");
		return 0;
	}
	else
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			//printf(YELLOW "is_valid_dir() found path %s\n" DEFAULT, path);
			return 1;
		}
	}
	return (0);
}

//ok!
int	check_existing(char **my_envp, const char *name)
{
	int	i;

	i = 0;
	while(my_envp[i])
	{
		if ((strncmp(my_envp[i], name, strlen(name)) == 0) && (my_envp[i][strlen(name)] == '='))
		{
			//printf(YELLOW "existing env : %s\n" DEFAULT, my_envp[i]);
			return (i);
		}
		i++;
	}
	//printf(RED " <%s> is non existing env\n" DEFAULT, name);
	return(-1);
}

int	get_env_count(char **my_envp)
{
	int env_count;

	env_count = 0;
	while(my_envp[env_count])
		env_count++;
	return env_count;
}

char	**expand_envp(t_shell *shell, char *new_path)
{
	int env_count;
	int	i;

	env_count = get_env_count(shell->my_envp);
	char	**new_envp = malloc(sizeof(char*) * (env_count + 2));
	if (!new_envp)
	{
		return(NULL);
	}
	i = 0;
	while(shell->my_envp[i])
	{
		new_envp[i] = shell->my_envp[i];
		i++;
	}
	new_envp[env_count] = new_path;
	new_envp[env_count + 1] = NULL;
	return (new_envp);
}

char *create_new_path(const char *name, const char *value)
{
	int	namelen;
	int	valuelen;

	if (!value)
	{
		namelen = ft_strlen(name);
		char *new_path = malloc(namelen + 1);
		if (!new_path)
		{
			perror("failed to create new_path");
			return (NULL);
		}
		ft_strlcpy(new_path, name, namelen + 1);
		// if(new_path[namelen] == '\0')
		// 	printf(RED"null\n"DEFAULT);
		return new_path;
	}
	else
	{
		namelen = ft_strlen(name);
		valuelen = ft_strlen(value);
		char *new_path = malloc(namelen + valuelen + 2);
		if (!new_path)
		{
			perror("failed to create new_path");
			return (NULL);
		}
		ft_strlcpy(new_path, name, namelen + valuelen + 2);
		new_path[namelen] = '=';
		ft_strlcpy(new_path + namelen + 1, value, valuelen + 1);
		return (new_path);
	}
}

int	ft_setenv(const char *name, const char *value, int overwrite, t_shell *shell)
{
	int		index;
	char	*new_path;
	char	**new_envp;

	if(!name || *name == '\0')
		return (0);
	index = check_existing(shell->my_envp, name);
	new_path = create_new_path(name, value);
	if (!new_path)
		return (0);
	//if already existed but, overwrite == 0
	if(index >= 0 && overwrite == 0)
		return (0);
	//if existed , and want to overwrite to it.
	if (index >= 0 && overwrite == 1)
	{
		free(shell->my_envp[index]);
		shell->my_envp[index] = new_path;
		return (1);
	}
	else
	{
		//if ist not existed then anyway we have to assign to it 
		new_envp = expand_envp(shell, new_path);
		if(!new_envp)
		{
			free(new_path);
			return 0;
		}
		free(shell->my_envp);
		shell->my_envp = new_envp;
		return (1);
	}
	return (0);
}


void cd(char **argv, t_shell *shell, t_gc_list *gc_list)
{
	shell->cur_dir = my_getcwd(shell, gc_list);
	// if (!shell->cur_dir)
	// {
	// 	perror(RED"get shell->cur_dir failed"DEFAULT);
	// 	return ;
	// }

	//test just cd 
	if ((strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2) && argv[2] == NULL)
	{
		printf(RED"case just cd\n"DEFAULT);
		char	*homedir;

		homedir = find_var_in_env(shell->my_envp, "HOME", 4, gc_list);
		if (chdir(homedir) != 0)
		{
			perror(RED "chdir error for 'cd'\n"DEFAULT);
			free(shell->cur_dir);
			return ;
		}
		char	*new_dir = my_getcwd(shell, gc_list);
		if (new_dir)
		{
			shell->old_dir = shell->cur_dir;
			ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
			shell->cur_dir = new_dir;
			ft_setenv("PWD", shell->cur_dir, 1, shell);
		}
		else
		{
			perror("getcwd error");
			//todo free allocated variables;
		}
	}

	//test cd -
	else if (strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2 && argv[2])
	{
		if(argv[2][0] == '-' && strlen(argv[2]) == 1)
		{
			printf(RED"get in for 'cd -'\n"DEFAULT);
			shell->old_dir = find_var_in_env(shell->my_envp, "OLDPWD", 6, gc_list);
			if (!shell->old_dir)
			{
				fprintf(stderr, RED"cd:OLDPWD not set\n"DEFAULT);
				//todo all free(shell->cur_dir);
				return ;
			}
			if (chdir(shell->old_dir) != 0)
			{
				perror(RED "chdir error for cd - \n"DEFAULT);
				//todo all free(shell->cur_dir);
				return ;
			}
			char	*new_dir = my_getcwd(shell, gc_list);
			if (new_dir)
			{
				shell->old_dir = shell->cur_dir;
				ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
				shell->cur_dir = new_dir;
				ft_setenv("PWD", shell->cur_dir, 1, shell);
			}
			else
			{
				perror("getcwd error");
				//free allocated variables;
			}
		}

		//test absolute path
		else if(is_valid_dir(argv[2]))
		{
			printf(RED"argv[2]%s\n"DEFAULT,argv[2]);
			if (chdir(argv[2]) != 0)
			{
				perror(RED "chdir error for 'argv[2]' in cd func\n"DEFAULT);
				free(shell->cur_dir);
			}
			char	*new_dir = my_getcwd(shell, gc_list);
			if (new_dir)
			{
				shell->old_dir = shell->cur_dir;
				ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
				shell->cur_dir = new_dir;
				ft_setenv("PWD", shell->cur_dir, 1, shell);
			}
			else
			{
				perror("getcwd error");
				//free allocated memories.
			}
		}
	}
	else
	{
		printf(RED"not found directory\n"DEFAULT);
	}
}


// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	t_gc_list *head = init_gc_list();
// 	t_shell *shell = init_shell_info();
	
// 	// int i = 0;
// 	// while (test[i])
// 	// {
// 	// 	printf(BLUE "test[%d], %s:\n" DEFAULT ,i, test[i]);
// 	// 	i++;
// 	// }
// 	//printf(YELLOW "head node %p\n",head);
// 	//print_list(head);
// 	//printf(BLUE "%p\n" DEFAULT, head->data);
// 	//printf(BLUE "%p\n" DEFAULT, head->next->data); DEFAULT;
	
// 	shell->my_envp = copy_envp(head, envp);
// 	//is_valid_dir("../../MINISHELL/EXECUTION");
// 	//check_existing(shell_info->my_envp, "OLDPWD");

// 	//test
// 	int i = 0;
// 	while (shell->my_envp[i])
// 	{
// 		printf(GREEN"%p,  %s\n"DEFAULT, shell->my_envp[i], shell->my_envp[i]);
// 		i++;
// 	}

// 	ft_setenv("HOME", YELLOW"MY TEST?"DEFAULT, 1, shell);
// 	ft_setenv(YELLOW "MYPWD" GREEN, YELLOW"??????????????????????"GREEN, 1, shell);
	
// 	// cd(argv, shell);

// 	//test
// 	i = 0;
// 	while (shell->my_envp[i])
// 	{
// 		printf(BLUE"%p,  %s\n"DEFAULT, shell->my_envp[i], shell->my_envp[i]);
// 		i++;
// 	}
// }