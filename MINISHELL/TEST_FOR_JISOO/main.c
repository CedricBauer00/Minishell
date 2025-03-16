#include "../INCLUDE/main.h"

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
	memset(new_path, 0, new_path_size);
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
		
		//test
		int k = 0;
		while(new_envp[k])
		{
			printf("new_envp[%d], %s\n", k, new_envp[k]);
			k++;
		}
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

	}
	if ("cd ~ || cd ")
	{

	}
	if ("cd ..")
	{

	}
	if ("cd -")
	{

	}
	if ("cd ../")
	{

	}
}

int main(int argc, char **envp)
{
	(void)argc;
	ft_setenv("mypwd", "im a oldpwd",1, envp);
	cd();
	return 0;
}