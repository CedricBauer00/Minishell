/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/06 10:54:54 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	prevent_zombie_process()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void	wait_for_child_and_update_status(int i)
{
	// t_shell	*shell = get_shell();
	t_shell	*shell;
	int		status;
	int		idx;

	idx = 0;
	shell = get_shell();
	if(!shell->pids)
		return;
	while(idx < i)
	{
		fprintf(stderr, RED"shell->pids[idx] %d\n"DEFAULT, shell->pids[idx]);
		pid_t child_pid = wait4(shell->pids[idx], &status, 0 ,NULL);
		fprintf(stderr ,RED"child_pid %d\n"DEFAULT, child_pid);
		fprintf(stderr ,BLUE"parent got this from wait4() child_pid : %d\n"DEFAULT, child_pid);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			fprintf(stderr, GREEN"exited with %d\n"DEFAULT, WEXITSTATUS(status));
			shell->last_status_exit = WEXITSTATUS(status);  //parents get exit status 
		}
		else if (WIFSIGNALED(status))
		{
			fprintf(stderr, RED"terminated by signal %d (%s)\n" DEFAULT, WTERMSIG(status), strsignal(WTERMSIG(status)));
			shell->last_status_exit = 128 + WTERMSIG(status);
		}
		idx++;
	}
}

//todo change name becuz it doenst make offset or maybe i can make it to have it offset 
int heredoc_fd_offset_and_redir(t_cmd_block *cur)
{
	t_shell	*shell;

	shell = get_shell();
	if (!cur)
		return -1;
	//fprintf(stderr, RED"in execute_child cur->io_streams->heredoc_fd: %d\n"DEFAULT, cur->io_streams->heredoc_fd);
	//fprintf(stderr, RED"in execute_child STDIN_FILENO: %d, STDOUT_FILENO: %d\n"DEFAULT, STDIN_FILENO, STDOUT_FILENO);
	shell->heredoc_fd = open("temp_heredoc" ,O_RDWR, 0644);
	//fprintf(stderr, RED"HEREDOR file open [pid %d] open(%d)\n"DEFAULT, getpid(), cur->io_streams->heredoc_fd);
	if (shell->heredoc_fd < 0)
	{
		return -1;
	}
	if (dup2(shell->heredoc_fd, STDIN_FILENO) == -1)
	{
		close(shell->heredoc_fd);
		unlink("temp_heredoc");
		return -1;
	}
	//fprintf(stderr, RED"HEREDOR REDIR[pid %d] dup2(%d, %d)\n"DEFAULT, getpid(), cur->io_streams->heredoc_fd, STDIN_FILENO);
	close(shell->heredoc_fd);
	unlink("temp_heredoc");
	return 1;
}

void	execute_builtin(t_cmd_block *cur, t_shell *shell)
{
	// t_gc *gc = get_gc();
	t_gc *gc;

	gc = get_gc();
	fprintf(stderr, GREEN"execute_builtin()\n"DEFAULT);
	if (strcmp(cur->built_in, "cd") == 0)
	{
		fprintf(stderr, RED"try to do 'cd' execute_builtin\n"DEFAULT);
		cd(cur->args, shell, gc);
	}
	if (strcmp(cur->built_in, "echo") == 0)
	{
		ft_echo(cur->args, shell);
	}
	if (strcmp(cur->built_in, "export") == 0)
	{
		export(cur->args, shell);
	}
	if (strcmp(cur->built_in, "pwd") == 0)
	{
		ft_pwd(cur->args, gc);
	}
	if (strcmp(cur->built_in, "env") == 0)
	{
		ft_env(cur->args, shell);
	}
	if (strcmp(cur->built_in, "unset") == 0)
	{
		ft_unset(cur->args, shell);
	}
}

void	single_cmd_execute(t_cmd_block *cur, t_gc *gc)
{
	t_shell	*shell;
	pid_t	pid;

	if (!cur)
		return;
	shell = get_shell();

	if (cur->io_streams)
		set_io_streams(cur);
	if (cur->is_built_in)
	{
		if (cur->io_streams && cur->io_streams->heredoc_eof)
		{
			if (heredoc_fd_offset_and_redir(cur) == -1)
			{
				perror(RED"heredoc error in single_cmd_execute()"DEFAULT);
				gc_free(gc);
				exit(1);
			}
		}
		execute_builtin(cur, shell);
	}
	else if (cur->is_external_cmd)
	{
		pid = fork();
		shell->pids[0] = pid;
		fprintf(stderr, YELLOW" singlecmd for child proc fork() : %d , shell->pids[0] %d \n"DEFAULT, pid, shell->pids[0]) ;
		if (pid == 0) //signal implementation?
		{
		
			if (cur->io_streams && cur->io_streams->heredoc_eof)
			{
				if (heredoc_fd_offset_and_redir(cur) == -1)
				{
					perror(RED"heredoc error in single_cmd_execute()"DEFAULT);
					gc_free(gc);
					exit(1);
				}
			}
			run_execve(cur, gc);
		}
		else
			wait_for_child_and_update_status(1);
	}
}

void	execute_child(t_cmd_block *cur, t_gc *gc, t_shell *shell)
{
	if (cur->io_streams && cur->io_streams->heredoc_eof)
	{
		if (heredoc_fd_offset_and_redir(cur) == -1)
		{
			perror("errror heredoc");
			gc_free(gc);
			exit(1);
		}
	}
	processing_pipe(cur);
	set_io_streams(cur);
	if (cur->built_in)
	{
		fprintf(stderr, RED"is it in pipe for execute_builtin execute_child()\n"DEFAULT);
		execute_builtin(cur, shell);
	}
	if (cur->args)
	{
		fprintf(stderr, RED"is it in pipe for external cmd execute_child()\n"DEFAULT);
		run_execve(cur, gc);
	}
}

void 	run_execve(t_cmd_block *cmd_block, t_gc *gc)
{
	char	**splitted_path;
	t_shell	*shell;

	shell = get_shell();
	char *path = find_var_in_env(shell->my_envp, "PATH", 4, gc->temp);
	if (!path)
	{
		perror(RED"can't find PATH"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	splitted_path = ft_split(path, ':');
	free(path);
	if (!splitted_path)
	{
		perror(RED"ft_split failed in run_execve()"DEFAULT);
		gc_free(gc);
		exit(1);
	}
	int i = 0;
	while(splitted_path[i])
	{
		char *attach_slash_to_cmd = gc_strjoin(splitted_path[i], "/", &gc->temp);
		if (!attach_slash_to_cmd)
		{
			gc_free(gc);
			exit(1);
		}
		char *cmd_path = gc_strjoin(attach_slash_to_cmd, cmd_block->args[0], &gc->temp);
		if (!cmd_path)
		{
			gc_free(gc);
			exit(1);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			signal(SIGINT, signal_handler_for_child);
			signal(SIGQUIT, signal_handler_for_child);
			fprintf(stderr, "execve()\n");
			if (execve(cmd_path , cmd_block->args, shell->my_envp) == -1)
			{
				gc_free(gc);
				perror(RED"error execve()"DEFAULT);
				exit(1);
			}
		}
		i++;
	}
	fprintf(stderr, RED"command not found: %s\n"DEFAULT, cmd_block->args[0]);
	exit(127);
}

void	main_execute(t_cmd_block *cmd_block)
{
	t_cmd_block	*cur;
	//int			fd;
	int			stdin_backup;
	int			stdout_backup;

	cur = cmd_block;
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	//hanlde_heredoc(cur);
	int	pid_counts = count_command(cmd_block);
	printf("pid_counts %d\n", pid_counts);
	do_alloc_pids(cmd_block);
	if (pid_counts == 1)
		execute_single_command(cur);
	if (pid_counts > 1)
	{
		execute_pipeline(cur);
	}
	prevent_zombie_process();
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	fprintf(stderr, RED"-CHECK ORGINAL STDIN AND STDOUT-\n STDIN_FILENO: %d, STDOUT_FILENO: %d\n"DEFAULT, STDIN_FILENO, STDOUT_FILENO);
}

// void	hanlde_heredoc(t_cmd_block *cmd_block)
// {
// 	t_cmd_block *cur = cmd_block;
// 	while(cur)
// 	{
// 		if (cur->io_streams && cur->io_streams->heredoc_eof)
// 		{
// 			process_heredoc(cur->io_streams);
// 			printf(RED"heredocfd %d\n"DEFAULT, cur->io_streams->heredoc_fd);
// 		}
// 		cur = cur->next;
// 	}
// }

void	execute_single_command(t_cmd_block *cmd_block)
{
	t_gc	*gc;

	gc = get_gc();
	if(cmd_block && !cmd_block->prev && !cmd_block->next)
	{
		fprintf(stderr, RED"this is execute_single_command()\n"DEFAULT);
		single_cmd_execute(cmd_block, gc);
	}
}

int	count_command(t_cmd_block *cmd_block)
{
	t_cmd_block	*temp;
	int			count;
	
	temp = cmd_block;
	count = 0;

	while(temp)
	{
		count++;
		temp = temp->next;
	}
	return count;
}

void	do_alloc_pids(t_cmd_block* cmd_block)
{
	int		count;
	t_shell	*shell;
	t_gc	*gc;

	gc = get_gc();
	count = count_command(cmd_block);
	if (count == 0 || (count == 1 && cmd_block->built_in))
		return ;
	shell = get_shell();
	shell->pids = do_alloc(&gc->temp, sizeof(pid_t) * count, TYPE_SINGLE_PTR, "pids");
	if(!shell->pids)
	{
		gc_free(gc);
		exit(1);
	}
	fprintf(stderr, YELLOW"pids is alloc in do_alloc_pids() counts: %d\n"DEFAULT, count);
}

void	fork_and_execute(t_cmd_block *cmd_block, t_gc *gc, int *i)
{
	t_cmd_block	*cur;
	pid_t		pid;
	t_shell		*shell;

	shell = get_shell();
	cur = cmd_block;
	//hanlde_heredoc(cur);
	//set_io_streams(cur);
	if (cur->next)
	{
		add_pipe(&cur);
		fprintf(stderr, YELLOW"[pid %d] p_pipe->pipefd[0]: %d, p_pipe->pipefd[1]: %d\n"DEFAULT,getpid(), cur->pipe->pipefd[0], cur->pipe->pipefd[1]);
	}
	pid = fork();
	fprintf(stderr,YELLOW"[pid %d] fork()\n"DEFAULT, getpid());
	if (pid == 0) //signal function call?
	{
		//set_io_streams(cur);
		execute_child(cur, gc, shell);
	}
	close_pipefd(cur);
	shell->pids[*i] = pid;
}

void	execute_pipeline(t_cmd_block *cmd_block)
{
	t_cmd_block	*cur;
	t_gc		*gc;
	int			i;

	i = 0;
	gc = get_gc();
	cur = cmd_block;
	while(cur)
	{
		fork_and_execute(cur, gc, &i);
		i++;
		cur = cur->next;
	}
	wait_for_child_and_update_status(i);
}
