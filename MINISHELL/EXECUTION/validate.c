# include "../INCLUDE/main.h"

/*
	if cur->type is '|' or '<', '>', ''>>
		if(!cur->next) 
			error;
	if (cur->type is '|')
		if(!cur->prev or !cur->next or cur->next == '|')
			error;

*/
//after

int	validate_syntax(t_token *token)
{
	t_token *cur;

	cur = token;
	while (cur)
	{
		if ((cur->type & (TOKEN_PIPE | TOKEN_APPEND | TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT)) &&
			(!cur->next || cur->next->type == TOKEN_PIPE || cur->next->type == TOKEN_NONE))
		{
			perror(RED"syntax error"DEFAULT);
			//todo all_Free 
		}

		//ensure that pipe is not the beginning or end
		if (cur->type == TOKEN_PIPE)
		{
			if(!cur->prev || !cur->next || cur->next->type == TOKEN_PIPE)
			{
				perror(RED"syntax error"DEFAULT);
			}
		}
		
		//heck if a TOKEN_WORD follows after a redirection
		if (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND))
		{
			if (!cur->next || cur->next->type != TOKEN_WORD)
			{
				perror(RED"syntax error"DEFAULT);
			}
		}

		//heck if a TOKEN_EOF follows after a TOKEN_HEREDOC
		if (cur->type == TOKEN_HEREDOC)
		{
			if(!cur->next)
			{
				perror(RED"syntax error"DEFAULT);
			}
		}

		//memo ich muss nicht es hier auschecken, weil TOKEN_FLAGS mit ACCESS funktion ueberprueft werden kann.
		// if (cur->type == TOKEN_CMD || cur->type == TOKEN_FLAGS)
		// {
		// 	cur->next file.
		// 	cur->next flags 
		// 	cur->next >>
		// 	cur->next <<
		// 	cur->next <
		// 	cur->next >
		// 	cur->next |
		// 	cur->next 
		// }
		cur = cur->next;
	}
}

int	validate_cmd_block(t_cmd_block *cmd_b)
{
	t_cmd_block *cur;

	cur = cmd_b;
	while(cur)
	{
		if (!cur->cmd && !cur->built_in)
		{
			perror(RED"no cmd validate_cmd_block()"DEFAULT);
			//todo all free
		}
		cur = cur->next;
	}
}
