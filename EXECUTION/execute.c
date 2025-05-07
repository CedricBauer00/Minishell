/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:54 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/07 14:47:56 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void    execute_pipeline(t_cmd_block *cmd_block)
{
    t_cmd_block *cur;
    t_gc        *gc;
    int         i;
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
void    execute_single_command(t_cmd_block *cmd_block)
{
    t_gc    *gc;
    gc = get_gc();
    if(cmd_block && !cmd_block->prev && !cmd_block->next)
    {
       // fprintf(stderr, RED"this is execute_single_command()\n"DEFAULT);
        single_cmd_execute(cmd_block, gc);
    }
}

void    main_execute(t_cmd_block *cmd_block)
{
    t_cmd_block *cur;
    int         stdin_backup;
    int         stdout_backup;
    cur = cmd_block;
    stdin_backup = dup(STDIN_FILENO);
    stdout_backup = dup(STDOUT_FILENO);
    int pid_counts = count_command(cmd_block);
   // printf("pid_counts %d\n", pid_counts);
    do_alloc_pids(cmd_block);
    if (pid_counts == 1)
        execute_single_command(cur);
    if (pid_counts > 1)
        execute_pipeline(cur);
    prevent_zombie_process();
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
    //fprintf(stderr, RED"-CHECK ORGINAL STDIN AND STDOUT-\n STDIN_FILENO: %d, STDOUT_FILENO: %d\n"DEFAULT, STDIN_FILENO, STDOUT_FILENO);
}

void    do_alloc_pids(t_cmd_block* cmd_block)
{
    int     count;
    t_shell *shell;
    t_gc    *gc;
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
   // fprintf(stderr, YELLOW"pids is alloc in do_alloc_pids() counts: %d\n"DEFAULT, count);
}

int count_command(t_cmd_block *cmd_block)
{
    t_cmd_block *temp;
    int         count;
    temp = cmd_block;
    count = 0;
    while(temp)
    {
        count++;
        temp = temp->next;
    }
    return count;
}