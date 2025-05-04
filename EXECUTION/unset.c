#include "execution.h"

/*
	unset delte path in env
	but couldn't delte readonly file.
	and after getting rid of "PATH" then find anymore env.

*/

//S_IWUSR	: write permmision for USER
//S_IWGRP	: write permission for GROUP
//S_IWOTH	: write permission for OTHERS
//this way just only for checking system vars.

// int	is_readonly(const char *path)
// {
// 	struct stat file_stat;

// 	if (stat(path, &file_stat) != 0)
// 	{
// 		perror("failed stat()");
// 		return 0;
// 	}
// 	if (!(file_stat.st_mode & (S_IWUSR | S_IWGRP | S_IWOTH)))
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

//todo think about multiple env NAME should work like "unset PWD OLDPWD HOME PATH"
void	ft_unset(char **args, t_shell *shell)
{
	int	i;
	//int	j;
	int	found;
	bool check;
	//t_gc *gc;

	//gc = get_gc();
	check = false;
	i = 0;
	if (args[i] != NULL)
	{
		check = is_valid_identifier(args[i]);
		{
			if (check == false)
			{
				perror(RED"non valid identifier\n"DEFAULT);
				return ;
			}
		}
		while (args[i])
		{
			found = -1;

			found = check_existing(shell->my_envp, args[i]);
			printf(GREEN"in ft_unset() %d\n"DEFAULT, found);
			if (found != -1)
			{
				t_gc *gc = get_gc();
				t_gc_list *find = find_node(gc->shell, (char*)shell->my_envp[found]);
				delete_node(&gc->shell, find);
				while(shell->my_envp[found + 1])
				{
					shell->my_envp[found] = shell->my_envp[found + 1];
					found++;
				}
				shell->my_envp[found] = NULL;
			}
			i++;
		}
	}
}


