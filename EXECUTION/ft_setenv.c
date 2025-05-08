#include "execution.h"

static void	overwrite_env(int index, char *new_path)
{
	t_gc_list	*find;
	t_shell		*shell;
	t_gc		*gc;

	gc = get_gc();
	shell = get_shell();

	shell->my_envp[index] = gc_strdup(new_path, &gc->shell);
	find = find_node(gc->temp, (char *)new_path);
	delete_node(&gc->temp, find);
	return ;
}

static void	make_env(char *new_path)
{
	t_shell		*shell;
	t_gc		*gc;
	char		**new_envp;
	
	new_envp = NULL;
	shell = get_shell();
	gc = get_gc();
	
	new_envp = expand_envp(shell, new_path);
	
	//todo maybe we have to change gc_strdup function or think about how to update it
	char **old_envp = shell->my_envp;
	t_gc_list *old_envp_node = find_node(gc->shell, (char **)old_envp);
	delete_node(&gc->shell, old_envp_node);
	shell->my_envp = new_envp;
}

void	ft_setenv(const char *name, const char *value, int overwrite, t_shell *shell)
{
	int			index;
	char		*new_path;
	t_gc		*gc;

	if (!name || *name == '\0')
		return ;
	gc = get_gc();
	index = check_existing(shell->my_envp, name);
	new_path = create_new_path(name, value);
	is_exited(new_path, gc);
	if (index >= 0 && overwrite == 0)
		return ;
	if (index >= 0 && overwrite == 1)
	{
		overwrite_env(index, new_path);
		// shell->my_envp[index] = gc_strdup(new_path, &gc->shell);
		// find = find_node(gc->temp, (char *)new_path);
		// delete_node(&gc->temp, find);
	}

	else if (index < 0)
	{
		make_env(new_path);
		// new_envp = expand_envp(shell, new_path);
		// old_envp = shell->my_envp;
		// shell->my_envp = new_envp;
		// old_envp_node = find_node(gc->shell, (char **)old_envp);
		// delete_node(&gc->shell, old_envp_node);

		
		// shell->my_envp = new_envp;
	}
	return ;
}