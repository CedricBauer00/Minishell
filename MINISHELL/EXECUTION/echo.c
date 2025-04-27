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

void	ft_echo(char **argv, t_shell *shell, t_gc_list *gc_list)
{
	int i  = 2;
	bool is_newline = true;
	if ((strcmp(argv[1], "echo") == 0))
	{
		while (argv[i] && strcmp(argv[i], "-n") == 0)
		{
			is_newline =false;
			i++;
		}
		while (argv[i] != NULL)
		{
			printf(YELLOW"%s", argv[i]);
			if (argv[i + 1] != NULL)
				printf(" ");
			char *temp = ft_strchr(argv[i], '$');
			if (temp != NULL)
			{
				char *var_name = temp + 1;
				char *var = find_var_in_env(shell->my_envp, var_name, ft_strlen(var_name), gc_list);
				// search it,,
				if (check_existing(shell->my_envp, var))
				{
					char *result = ft_strdup(shell->my_envp[ft_strlen(var)]);
					free (result);
				}
				else
				{
					printf("\n");
				}
			}
			i++;
		}
		if (is_newline)
			printf("\n"DEFAULT);
	}
	
}


