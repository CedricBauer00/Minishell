#include "execution.h"

//todo make export.
// char	**expand_envp(t_shell *shell, char *new_path)
// char *create_new_path(const char *name, const char *value)

//export even  if no existed '=' it should works
//if i use only with '=' we can see that var in env
void	print_envp(t_shell *shell)
{
	int	i;
	bool is_path = false;
	int	j;
	t_gc *gc;

	i = 0;
	j = 0;
	if (!shell)
		return ;

	gc = get_gc();
	is_path = check_existing(shell->my_envp, "PATH");
	while(shell->my_envp[i])
	{
		if (extract_name(shell->my_envp[i]) && extract_value(shell->my_envp[i]) == NULL)
			printf(YELLOW"declare -x %s\n"DEFAULT, extract_name(shell->my_envp[i]));
		else
			printf(YELLOW"declare -x %s=\"%s\"\n"DEFAULT, extract_name(shell->my_envp[i]), extract_value(shell->my_envp[i]));
		i++;
	}
}

static bool	is_valid_identifier(const char *name)
{
	int	i;

	i = 0;
	if(!name || (!ft_isalpha(name[0]) && name[0] != '_'))
	{
		return false;
	}
	while(name[i])
	{
		if(!ft_isalpha(name[i]) && name[i] != '_')
			return false;
		i++;
	}
	return true;
}

//todo fix must dont get digits.
void	export(char **args, t_shell *shell)
{
	t_gc *gc;
	int	i;
	bool check;

	i = 0;
	gc = get_gc();
	if (args && args[0] == NULL)
	{
		print_envp(shell);
	}
	while ((args[i]))
	{
		check = is_valid_identifier(args[i]);
		if (check == false)
		{
			perror(RED"non valid identifier\n"DEFAULT);
			//all_free(&gc->temp);
		}
		char	*name = extract_name(args[i]);
		is_exited(name, gc);
		printf("name:%s\n", name);
		char	*value = extract_value(args[i]);
		// is_exited(value, gc);
		printf("value:%s\n", value);
		ft_setenv(name, value, 1 , shell);
		free(name);
		free(value);
		i++;
	}
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
