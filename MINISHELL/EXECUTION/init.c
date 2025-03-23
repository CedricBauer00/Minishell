# include "../INCLUDE/main.h"

t_shell	*init_shell_info(void)
{
	t_shell *shell_info;
	shell_info = malloc(sizeof(t_shell));
	if (!shell_info)
	{
		return (NULL);
	}
	shell_info->cur_dir = NULL;
	shell_info->old_dir = NULL;
	shell_info->my_envp = NULL;
	shell_info->last_status_exit = 0;
	return shell_info;
}

//after using this function at the beginning, this func ensures that the 'shell' is no longer 'null'
//because it has already been initialized as a static var.

t_shell *get_shell(void)
{
	static t_shell *shell = NULL;
	if (shell == NULL)
	{
		shell = init_shell_info();
	}
	return (shell);
}