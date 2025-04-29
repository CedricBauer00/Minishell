# include "../INCLUDE/main.h"

/*
	echo $ //using VAR
	echo -n -n -n //ok!
	echo "HELLO"
	
	//todo
	echo $? >>> printf("shell ->last_exit");
	case for ''
	case for ""
*/

void	ft_echo(char **args, t_shell *shell)
{
	int i  = 0;
	bool is_newline = true;
	t_gc	*gc;

	gc = get_gc();

	if (ft_strncmp(args[0], "$", 1) == 0)
	{
		//char *temp = ft_strdup(args[0] + 1);
		if (*args + 1)
		{
			char *var_name = *args + 1;
			char *var = find_var_in_env(shell->my_envp, var_name, ft_strlen(var_name), gc->temp);
			if (!var)
			{
				gc_free(gc);
				exit(1);
			}
			printf(YELLOW"%s\n"DEFAULT, var);
			// search it,,
			// if (check_existing(shell->my_envp, var))
			// {
			// 	char *result = ft_strdup(shell->my_envp[ft_strlen(var)]);
			// 	printf(BLUE"%s\n"DEFAULT, result);
			// 	free (result);
			// 	exit(0);
			// }
			// else
			// {
			printf("\n");
			//}
		}
	}
	else
	{
		while (args[i] && strcmp(args[i], "-n") == 0)
		{
			is_newline = false;
			i++;
		}
		while (args[i] != NULL)
		{
			printf(YELLOW"%s", args[i]);
			if (args[i + 1] != NULL)
				printf(" ");
			i++;
		}
	}
	if (is_newline)
		printf("\n"DEFAULT);
}


