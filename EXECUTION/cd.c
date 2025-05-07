/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:23 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 12:23:42 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_valid_dir(const char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) != 0)
	{
		perror("failed stat()");
		return (0);
	}
	else
	{
		if (S_ISDIR(file_stat.st_mode))
			return (1);
	}
	return (0);
}

int	check_existing(char **my_envp, const char *name)
{
	int	i;

	i = 0;
	while (my_envp[i])
	{
		if ((strncmp(my_envp[i], name, strlen(name)) == 0)
			&& (my_envp[i][strlen(name)] == '='))
			return (i);
		i++;
	}
	return (-1);
}

int	get_env_count(char **my_envp)
{
	int	env_count;

	env_count = 0;
	while (my_envp[env_count])
		env_count++;
	return (env_count);
}

char	**expand_envp(t_shell *shell, char *new_path)
{
	int		env_count;
	int		i;
	t_gc	*gc;
	char	**new_envp;

	gc = get_gc();
	env_count = get_env_count(shell->my_envp);
	new_envp = malloc(sizeof(char *) * (env_count + 2));
	if (!new_envp)
	{
		gc_free(gc);
		exit(1);
	}
	i = 0;
	while (shell->my_envp[i])
	{
		new_envp[i] = shell->my_envp[i];
		i++;
	}
	new_envp[env_count] = new_path;
	new_envp[env_count + 1] = NULL;
	return (new_envp);
}

char	*create_new_path(const char *name, const char *value)
{
	int		namelen;
	int		valuelen;
	t_gc	*gc;
	char	*new_path;

	gc = get_gc();
	if (!value)
	{
		namelen = ft_strlen(name);
		new_path = do_alloc(&gc->shell, namelen + 1, \
			TYPE_SINGLE_PTR, "new_path");
		if (!new_path)
		{
			perror("failed to create new_path");
			return (NULL);
		}
		ft_strlcpy(new_path, name, namelen + 1);
		return (new_path);
	}
	else
	{
		namelen = ft_strlen(name);
		valuelen = ft_strlen(value);
		new_path = do_alloc(&gc->shell, namelen + valuelen + 2, \
			TYPE_SINGLE_PTR, "new_path");
		if (!new_path)
		{
			perror("failed to create new_path");
			return (NULL);
		}
		ft_strlcpy(new_path, name, namelen + valuelen + 2);
		new_path[namelen] = '=';
		ft_strlcpy(new_path + namelen + 1, value, valuelen + 1);
		return (new_path);
	}
}

void	ft_setenv(const char *name, const char *value, \
	int overwrite, t_shell *shell)
{
	int			index;
	char		*new_path;
	char		**new_envp;
	t_gc		*gc;
	t_gc_list	*find;
	t_gc_list	*old_envp_node;
	char		**old_envp;

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
		shell->my_envp[index] = gc_strdup(new_path, &gc->shell);
		find = find_node(gc->temp, (char *)new_path);
		delete_node(&gc->temp, find);
	}
	else if (index < 0)
	{
		new_envp = expand_envp(shell, new_path);
		old_envp = shell->my_envp;
		shell->my_envp = new_envp;
		old_envp_node = find_node(gc->shell, (char **)old_envp);
		delete_node(&gc->shell, old_envp_node);
		shell->my_envp = new_envp;
	}
}

void	cd(char **args, t_shell *shell, t_gc *gc)
{
	char		*target;
	char		*new_dir;
	t_gc_list	*find;

	if (!shell)
		return ;
	shell->cur_dir = my_getcwd(gc);
	if (!shell->cur_dir)
	{
		perror(RED"get shell->cur_dir failed"DEFAULT);
		gc_free(gc);
		return ;
	}
	if (args[0] == NULL || strcmp(args[0], "~") == 0)
	{
		fprintf(stderr, RED"case just cd\n"DEFAULT);
		target = find_var_in_env(shell->my_envp, "HOME", 4, gc->temp);
		if (!target)
		{
			perror(RED "chdir error for 'cd'\n"DEFAULT);
			gc_free(gc);
			exit(1);
		}
	}
	else if (strcmp(args[0], "-") == 0)
	{
		fprintf(stderr, RED"for 'cd -'\n"DEFAULT);
		target = find_var_in_env(shell->my_envp, "OLDPWD", 6, gc->temp);
		if (!target)
		{
			fprintf(stderr, RED"cd:OLDPWD not set\n"DEFAULT);
			gc_free(gc);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, RED"for 'cd others'\n"DEFAULT);
		target = gc_strdup(args[0], &gc->temp);
		is_exited(target, gc);
	}
	if (chdir(target) != 0)
	{
		perror(RED"cd () error"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	new_dir = my_getcwd(gc);
	if (!new_dir)
	{
		perror(RED "chdir error for cd - \n"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	find = find_node(gc->temp, (char *)target);
	delete_node(&gc->temp, find);
	shell->old_dir = shell->cur_dir;
	ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
	fprintf(stderr, RED"sell->old_dir : %s\n"DEFAULT, shell->old_dir);
	shell->cur_dir = new_dir;
	ft_setenv("PWD", shell->cur_dir, 1, shell);
	fprintf(stderr , RED"sell->cur_dir : %s\n"DEFAULT, shell->cur_dir);
}
