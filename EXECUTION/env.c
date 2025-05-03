#include "execution.h"

void	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	while(shell->my_envp[i])
	{
		if(extract_name(shell->my_envp[i]) && extract_value(shell->my_envp[i]))
			printf(YELLOW"%s=%s\n"DEFAULT, extract_name(shell->my_envp[i]), extract_value(shell->my_envp[i]));
		i++;
	}
}