#include "execution.h"

void	ft_env(char **args, t_shell *shell)
{
	int		i;
	t_gc	*gc;

	gc = get_gc();
	i = 0;
	if (*args)
	{
		printf(RED"ENV TOO MANY ARGS\n"DEFAULT);
		all_free(&gc->temp);
		return ;
	}
	while(shell->my_envp[i])
	{
		if(extract_name(shell->my_envp[i]) && extract_value(shell->my_envp[i]))
			printf(YELLOW"%s=%s\n"DEFAULT, extract_name(shell->my_envp[i]), extract_value(shell->my_envp[i]));
		i++;
	}
}