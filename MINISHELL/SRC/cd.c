/*
	cd[path] : move to path;
	cd or cd ~ : move to home;
	cd -: move to prev dir;
	cd ..: move to parent dir;
	cd /: move to root dir;
*/

/*
	think!

	1. it mighte need to copy of envp
	2. and then maybe i need to stroe copied envp to a structure.
	3. and if we need a copied envp then change the var's name to copied envp;
*/
#include "../INCLUDE/main.h"


//stat(): check path, and check if its approchable.
//S_ISDIR is a macro 
//S_ISFIFO macro to check is pipe?
//S_IFREG macro to check is normal fiel?
//S_IFLNK	simbolick link
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
			return 1;
		}
	}
	return (0);
}

int	check_existing(char **envp, const char *name)
{
	int	i;

	i = 0;
	while(envp[i])
	{
		if ((strncmp(envp[i], name, strlen(name)) == 0) && (envp[i][strlen(name)] == '='))
		{
			printf("existing env\n");
			return (i);
			break;
		}
		i++;
	}
	return(0);
}

int	ft_setenv(const char *name, const char *value, int overwrite, char **envp)
{
	if(!value || !name || *name == '\0')
		return (0);
	int	index = check_existing(envp, name);
	//if already existed but, overwrite == 0
	if(index >= 0 && overwrite == 0)
	{
		printf("already existed but you dont want to replace value to it?\n");
		return (0);
	}
	//if not existed , then we need a new entry, but must consider of '=' and NULL
	size_t	new_path_size = strlen(value) + strlen(name) + 2;
	char	*new_path = malloc(sizeof(char) * (new_path_size));
	// memset(new_path, 0, new_path_size);
	if (!new_path)
	{
		perror("new entry malloc failed\n");
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
		printf("i:%d\n",i);
		i = 0;
		while(value[i])
		{
			new_path[namelen + i] = value[i];
			i++;
		}
		new_path[namelen + i] = '\0';
		envp[index] = new_path;
		printf("envp[index] %s\n",envp[index]);
		return 1;
	}
	//if ist not existed then anyway we have to assign to it 
	else
	{
		int i = 0;
		while(envp[i])
			i++;
		char	**new_envp = malloc(sizeof(char*) * i + 2);
		if (!new_envp)
		{
			free(new_path);
			perror("new_envp malloc failed\n");
			exit(1);
		}
		int j = 0;
		while(envp[j])
		{
			new_envp[j] = envp[j];
			j++;
		}
		new_envp[j] = new_path;
		new_envp[j + 1] = NULL;
		envp = new_envp;
	}
	return (0);
}

void cd()
{
	char	*curdir;
	char	*homedir;

	curdir = NULL;
	homedir = getenv("HOME");
	printf("homedir :%s\n", homedir);
	if (homedir == NULL)
	{
		perror("not found homedir\n");
	}
	curdir = getcwd(NULL, 0);
	if (curdir != NULL)
	{
		printf("curdir :%s\n", curdir);
	}
	else
	{
		perror("getcwd for curdir failed\n");
	}
	if ("cd <dirname>")
	{
		/*
			1.call stat() check if the dirname is valid
			2.change curpwd to oldpwd.
			3.call chdir() change to curdir
			4.call getcwd() assign to curdir
		*/
		curdir = getcwd(NULL, 0);
		char *input = "<dirname>";
		char *add_slash = ft_strjoin(curdir, "/");
		char *full_path = ft_strjoin(add_slash, input);
		free(add_slash);

		if (full_path == NULL)
		{
			perror("ft_strjoin error");
			free(curdir);
		}
		if (is_valid_dir(full_path))
		{
			printf("validpath");
			setenv("OLDPWD", curdir, 1, envp);
			free(curdir);
			if (chdir(full_path) != 0)
			{
				perror("chdir error for cd ~ || cd \n");
				free(full_path);
				//free allocated variables;
			}
			curdir = getcwd(NULL, 0);
			if (curdir != NULL)
			{
				setenv("PWD", curdir, 1, envp);
				free(curdir);
			}
			else
			{
				perror("getcwd error");
				//free allocated variables;
			}
		}
		else
		{
			perror("not found file");
		}
		free (full_path);	
	}

	//move to home?
	if ("cd ~ || cd ")
	{
		setenv("OLDPWD", curdir, 1, envp);
		if (chdir(homedir) != 0)
		{
			perror("chdir error for cd ~ || cd \n");
			//free allocated variables;
		}
		curdir = getcwd(NULL, 0);
		if (curdir != NULL)
		{
			setenv("PWD", curdir, 1, envp);
			free(curdir);
		}
		else
		{
			perror("getcwd error");
			//free allocated variables;
		}
	}

	//move to parent
	if ("cd ..")
	{
		setenv("OLDPWD", curdir, 1, envp);
		if (chdir("..") != 0)
		{
			perror("chdir error for cd ~ || cd \n");
			//free allocated variables;
		}
		curdir = getcwd(NULL, 0);
		if (curdir != NULL)
		{
			setenv("PWD", curdir, 1, envp);
			free(curdir);
		}
		else
		{
			perror("getcwd error");
			//free allocated variables;
		}
	}
	if ("cd -")
	{

	}
	if ("cd ../")
	{

	}
}

