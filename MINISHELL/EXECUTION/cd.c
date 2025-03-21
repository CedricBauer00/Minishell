/*
	cd[path] : move to path;
	cd or cd ~ : move to home;
	cd -: move to prev dir;
	cd ..: move to parent dir;
	cd /: move to root dir;
*/

/*
	think!
	1. i think, i need to copy of envp
	2. and then maybe i need to stroe copied envp to a structure.
	3. and if we need a copied envp then change the var's name to copied envp;
*/
#include "../INCLUDE/main.h"

//stat(): check path, and check if its approchable.
//S_ISDIR is a macro 
//S_ISFIFO macro to check is pipe?
//S_IFREG macro to check is normal fiel?
//S_IFLNK	simbolick link


/*
	//todo
	make structure for my_envp!
*/

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
			printf(YELLOW "is_valid_dir() found path %s\n" DEFAULT, path);
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
			printf(YELLOW "existing env : %s\n" DEFAULT, my_envp[i]);
			return (i);
		}
		i++;
	}
	printf(RED " <%s> is non existing env\n" DEFAULT, name);
	return(-1);
}

int	ft_setenv(const char *name, const char *value, int overwrite, t_shell *shell)
{
	if(!value || !name || *name == '\0')
		return (0);
	int	index = check_existing(shell->my_envp, name);
	//printf("index : %d\n",index);
	//if already existed but, overwrite == 0
	if(index >= 0 && overwrite == 0)
	{
		printf(YELLOW "already existed but you dont want to replace value to it?\n" DEFAULT);
		return (0);
	}
	//if not existed , then we need a new entry, but must consider of '=' and NULL
	size_t	new_path_size = strlen(value) + strlen(name) + 2;
	char	*new_path = malloc(sizeof(char) * (new_path_size));
	// memset(new_path, 0, new_path_size);
	if (!new_path)
	{
		perror(YELLOW "new entry malloc failed\n" DEFAULT);
		exit(1);
	}
	//if existed , and want to overwrite to it.
	if(index >= 0 && overwrite == 1)
	{
		int i = 0;
		while (name[i])
		{
			new_path[i] = name[i];
			i++;
		}
		int namelen= i;
		new_path[namelen] = '=';
		namelen++;
		i = 0;
		while(value[i])
		{
			new_path[namelen + i] = value[i];
			i++;
		}
		new_path[namelen + i] = '\0';
		shell->my_envp[index] = new_path;
		printf( "envp[%d],%s\n", index, shell->my_envp[index]);
		return 1;
	}
	//if ist not existed then anyway we have to assign to it 
	else if (index == -1 && (overwrite == 0 || overwrite == 1))
	{
		int i = 0;
		while(shell->my_envp[i])
			i++;
		printf(BLUE"%d\n"DEFAULT, i);
		char	**new_envp = malloc(sizeof(char*) * (i + 2));
		if (!new_envp)
		{
			free(new_path);
			perror("new_envp malloc failed\n");
			exit(1);
		}
		int j = 0;
		while(shell->my_envp[j])
		{
			new_envp[j] = shell->my_envp[j];
			j++;
		}

		i = 0;
		while (name[i])
		{
			new_path[i] = name[i];
			i++;
		}
		int namelen= i;
		new_path[namelen] = '=';
		namelen++;
		i = 0;
		while(value[i])
		{
			new_path[namelen + i] = value[i];
			i++;
		}
		new_path[namelen + i] = '\0';

		new_envp[j] = new_path;
		printf(BLUE" j: %d , %s\n"DEFAULT, j, new_envp[j]);
		new_envp[j + 1] = NULL;
		shell->my_envp = new_envp;
		return(1);
	}
	return (0);
}


void cd(char **argv, t_shell *shell)
{
	char	*homedir;

	shell->cur_dir = getcwd(NULL, 0);
	if (shell->cur_dir != NULL)
	{
		printf(YELLOW "curdir :%s\n" DEFAULT, shell->cur_dir);
	}
	else
	{
		perror("getcwd for curdir failed\n");
	}
	//absolute path
	if (strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2 && strcmp(argv[2],"/") == 0)
	{
		printf(GREEN "absolute path\n"DEFAULT);
		if(is_valid_dir(argv[2]))
		{
			printf(GREEN "validpath\n" DEFAULT);
			if (ft_setenv("OLDPWD", shell->cur_dir , 1, shell) != 1)
				printf(RED "ft_setenv error\n" DEFAULT);
			free(shell->cur_dir);
			if (chdir(argv[2]) != 0)
			{
				perror("chdir error for cd ~ || cd \n");
				//free allocated variables;
			}
			shell->cur_dir = getcwd(NULL, 0);
			if (shell->cur_dir != NULL)
			{
				ft_setenv("PWD", shell->cur_dir, 1, shell);
				printf(BLUE "curdir : %s\n" DEFAULT, shell->cur_dir);
				free(shell->cur_dir);
			}
			else
			{
				perror("getcwd error");
				//free allocated variables;
			}
		}
	}
	else
	{
		printf(RED"not cd <absolute path>\n"DEFAULT);
	}

	//test move to home?
	if  (strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2 && !argv[2] || 
		(strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2 && strcmp(argv[2], "~") == 0))
	{
		printf(BLUE"cd ~\n"DEFAULT);
		homedir = getenv("HOME");
		shell->cur_dir = getcwd(NULL, 0);
		printf(YELLOW "shell->curdir :%s\n" DEFAULT, shell->cur_dir);
		printf(YELLOW "homedir :%s\n" DEFAULT, homedir);
		if (homedir == NULL)
		{
			perror(RED "not found homedir\n" DEFAULT);
		}
		ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
		free(shell->cur_dir);
		shell->cur_dir = homedir;
		if (chdir(homedir) != 0)
		{
			perror("chdir error for cd ~ || cd \n");
			//free allocated variables;
		}
		printf(YELLOW "%s\n"DEFAULT, shell->cur_dir);
		if (shell->cur_dir != NULL)
		{
			ft_setenv("PWD", shell->cur_dir, 1, shell);
			//free(shell->cur_dir);
		}
		else
		{
			perror("getcwd error");
			//free allocated variables;
		}
	}
	else
	{
		printf(RED"not cd ~\n"DEFAULT);
	}

	// test move to parent
	if (strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2 && 
		(strcmp(argv[2], "..") == 0 || strncmp(argv[2], "../", 3) == 0))
	{
		printf("%s\n", shell->cur_dir);
		ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
		if (chdir("..") != 0)
		{
			perror("chdir error for cd ~ || cd \n");
			//free allocated variables;
		}
		shell->cur_dir = getcwd(NULL, 0);
		if (shell->cur_dir != NULL)
		{
			ft_setenv("PWD", shell->cur_dir, 1, shell);
			free(shell->cur_dir);
		}
		else
		{
			perror("getcwd error");
			//free allocated variables;
		}
	}
	else
	{
		printf(RED"not .. or ../\n"DEFAULT);
	}

	if (strcmp(argv[1], "cd") == 0 && strlen(argv[1]) == 2 && strcmp(argv[2], "-") == 0)
	{
		shell->old_dir = getenv("OLDPWD");
		if (!shell->old_dir)
		{
			perror(RED"cd: OLDPWD not set"DEFAULT);
		}
		printf(RED"shell->old_dir:%s\n"DEFAULT, shell->old_dir);
		if (chdir("..") != 0)
		{
			perror("chdir error for cd ~ || cd \n");
			//free allocated variables;
		}
		shell->cur_dir = shell->old_dir;
		if (shell->cur_dir != NULL)
		{
			ft_setenv("PWD", shell->cur_dir, 1, shell);
			free(shell->cur_dir);
		}
		else
		{
			perror("getcwd error");
			//free allocated variables;
		}
	}

	// if ("cd ../")
	// {

	// }
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_gc_list *head = init_gc_list();
	t_shell *shell = init_shell_info();
	
	// int i = 0;
	// while (test[i])
	// {
	// 	printf(BLUE "test[%d], %s:\n" DEFAULT ,i, test[i]);
	// 	i++;
	// }
	//printf(YELLOW "head node %p\n",head);
	//print_list(head);
	//printf(BLUE "%p\n" DEFAULT, head->data);
	//printf(BLUE "%p\n" DEFAULT, head->next->data); DEFAULT;
	
	shell->my_envp = copy_envp(head, envp);
	//is_valid_dir("../../MINISHELL/EXECUTION");
	//check_existing(shell_info->my_envp, "OLDPWD");

	// ft_setenv("HOME", YELLOW"MY TEST?"DEFAULT, 1, shell);
	// ft_setenv(YELLOW "MYPWD" GREEN, YELLOW"??????????????????????"GREEN, 1, shell);
	
	cd(argv, shell);

	//test
	int i = 0;
	while (shell->my_envp[i])
	{
		printf(GREEN"%p,  %s\n"DEFAULT,shell->my_envp, shell->my_envp[i]);
		i++;
	}
}