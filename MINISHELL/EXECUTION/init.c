# include "../INCLUDE/main.h"

t_shell_info	*init_shell_info(void)
{
	t_shell_info *shell_info;
	shell_info = malloc(sizeof(t_shell_info));
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
