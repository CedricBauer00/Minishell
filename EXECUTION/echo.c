#include "execution.h"

// void	echo_helper2(char **args, int i, bool is_newline)
// {
// 	while (args[i] && strcmp(args[i], "-n") == 0)
// 	{
// 		is_newline = false;
// 		i++;
// 	}
// 	while (args[i] != NULL)
// 	{
// 		printf(YELLOW"%s", args[i]);
// 		if (args[i + 1] != NULL)
// 			printf(" ");
// 		i++;
// 	}
// }

// int	echo_helper(char **args, t_shell *shell, int i, bool is_newline)
// {
// 	t_gc	*gc;
// 	char	*var_name;
// 	char	*var;

// 	gc = get_gc();
// 	if (ft_strncmp(args[0], "$", 1) == 0)
// 	{
// 		if (*args + 1)
// 		{
// 			var_name = *args + 1;
// 			var = find_var_in_env(shell->my_envp, var_name, 
// 				ft_strlen(var_name), gc->temp);
// 			if (!var)
// 			{
// 				gc_free(gc);
// 				exit(1);
// 			}
// 			printf(YELLOW"%s\n"DEFAULT, var);
// 		}
// 	}
// 	else
// 		echo_helper2(args, i, is_newline);
// 	return (0);
// }

// void	ft_echo(char **args, t_shell *shell)
// {
// 	int		i;
// 	bool	is_newline;

// 	i = 0;
// 	is_newline = true;
// 	if (!args[0])
// 	{
// 		printf("\n");
// 		return ;
// 	}
// 	if (echo_helper(args, shell, i, is_newline) == 1)
// 		return ;
// 	if (is_newline)
// 		printf("\n"DEFAULT);
// }


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
	fprintf(stderr, RED"try to do 'echo' ft_echo() \n"DEFAULT);
	if (ft_strncmp(args[0], "$", 1) == 0)
	{
		if (args[1])
		{
			char *var_name = *args + 1;
			char *var = find_var_in_env(shell->my_envp, var_name, ft_strlen(var_name), gc->temp);
			if (!var)
			{
				// all_free(&gc->temp);
				// is_newline = true;
				printf(YELLOW"\n"DEFAULT);
			}
		}
	}
	else if (args[i])
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
		printf(YELLOW"\n"DEFAULT);
}


