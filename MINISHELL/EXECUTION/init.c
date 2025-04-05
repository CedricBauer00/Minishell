# include "../INCLUDE/main.h"


t_shell	*init_shell_struct(void)
{
	t_shell *shell;
	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		return (NULL);
	}
	shell->cur_dir = NULL;
	shell->old_dir = NULL;
	shell->my_envp = NULL;
	shell->last_status_exit = 0;
	return shell;
}

//after using this function at the beginning, this func ensures that the 'shell' is no longer 'null'
//because it has already been initialized as a static var.
//static var is initialized just only at once when it called and assigned in data area.
t_shell *get_shell(void)
{
	static t_shell *shell = NULL;
	if (shell == NULL)
	{
		shell = init_shell_struct();
	}
	return (shell);
}
