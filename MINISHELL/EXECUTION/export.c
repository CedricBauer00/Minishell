#include "../INCLUDE/main.h"

//todo make export.
// char	**expand_envp(t_shell *shell, char *new_path)
// char *create_new_path(const char *name, const char *value)

void	print_envp(t_shell *shell, char *flag)
{
	int	i;

	i = 0;
	if (!shell)
		return ;
	while(shell->my_envp[i])
	{
		if (strcmp(flag, "export") == 0)
			printf("declare -x %s\n", shell->my_envp[i]);
		else if (strcmp(flag, "env") == 0)
			printf("%s\n", shell->my_envp[i]);
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
		char	*value = extract_value(argv[2]);
		if (!name || !value)
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

