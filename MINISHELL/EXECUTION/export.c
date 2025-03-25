#include "../INCLUDE/main.h"

//todo make export.
// char	**expand_envp(t_shell *shell, char *new_path)
// char *create_new_path(const char *name, const char *value)

// char	*ft_strchr(char *str, char c)
// {
// 	if (!str || !c)
// 		return NULL;
// 	while(1)
// 	{
// 		if(*str == c)
// 			return str;
// 		if(*str == '\0')
// 			break;
// 		str++;
// 	}
// 	return (NULL);
// }

void	print_envp(t_shell *shell, char *flag)
{
	int	i;

	i = 0;

	if (!shell)
		return ;
	while(shell->my_envp[i])
	{
		if (strcmp(flag, "export") == 0)
			printf(YELLOW"declare -x %s\n"DEFAULT, shell->my_envp[i]);
		else if (strcmp(flag, "env") == 0)
		{
			int j = 0;
			bool is_path = false;
			while (shell->my_envp[j] != NULL)
			{
				if (strncmp(shell->my_envp[j], "PATH=", 5) == 0)
				{
					is_path = true;
					break;
				}
				j++;
			}
			if (is_path)
			{
				j = 0;
				while (shell->my_envp[j] != NULL)
				{
					if(ft_strchr(shell->my_envp[j], '=') != NULL)
					{
						printf(BLUE"%s\n"DEFAULT, shell->my_envp[j]);
					}
					j++;
				}
			}
			else
			{
				printf(RED"env: No such file or directory\n"DEFAULT);
			}
		}
		i++;
	}
}

int	export(char **argv, t_shell *shell)
{
	if ((strcmp(argv[1], "export") == 0 )&& argv[2] == NULL)
	{
		print_envp(shell, argv[1]);
		return (1);
	}
	else if (strcmp(argv[1], "export") == 0 && argv[2] != NULL)
	{
		char	*name = extract_name(argv[2]);
		printf("name:%s\n", name);
		char	*value = extract_value(argv[2]);
		printf("value:%s\n", value);
		if (!name)
			return (0);
		else
		{
			int result = ft_setenv(name, value, 1 , shell);
			free(name);
			free(value);
			return (result);
		}
		
	}
	return 0;
}

char	*extract_name(char *arg)
{
	int	i;

	i = 0;
	while(arg[i])
	{
		if (arg[i] == '=')
			break;
		i++;
	}
	return ft_substr(arg, 0, i);
}

char	*extract_value(char *arg)
{
	int	i;

	i = 0;
	while(arg[i] != '\0' && arg[i] != '=')
	{
		i++;
	}
	if (arg[i] == '=')
		return ft_strdup(arg + i + 1);
	else
		return NULL;
}
