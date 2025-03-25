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
void	ft_echo(char **argv, t_shell *shell)
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
			if (argv[i + 1] != NULL)				//need to change to tokentype WORD
				printf(" ");
			char *temp = ft_strchr(argv[i], '$');
			if (temp != NULL)
			{
				char *var_name = temp + 1;
				char *var = getenv("var_name");
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


