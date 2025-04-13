#include "../INCLUDE/main.h"

t_cmd_block *add_command(t_gc_list *gc_lst, char *cmd, char **args, t_token_type type)
{
	t_cmd_block *new_cmd = init_command_struct(gc_lst);
	if (!new_cmd)
	{
		perror(RED"add_command()failed"DEFAULT);
		return NULL;
	}
	new_cmd->args = args;
	new_cmd->cmd = cmd;
	new_cmd->type = type;
	return new_cmd;
}