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

	// | <<
	cur = token;
	while (cur)
	{
		if ((cur->type & (TOKEN_PIPE | TOKEN_APPEND | TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT)) &&
			(!cur->next || cur->next->type == TOKEN_PIPE || cur->next->type == TOKEN_NONE))
		{
			perror(RED"syntax error"DEFAULT);
			//todo all_Free 
		}
		//ensure that pipe is not the beginning
		//todo think , what if pipe is at the end
		else if (cur->type == TOKEN_PIPE)
		{
			// if(!cur->prev || !cur->next || cur->next->type == TOKEN_PIPE)
			// {
			// 	perror(RED"syntax error"DEFAULT);
			// }
			if(!cur->prev || cur->next->type == TOKEN_PIPE)
			{
				perror(RED"syntax error"DEFAULT);
			}
		}

		//heck if a TOKEN_WORD follows after a redirection
		else if (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND))
		{
			if (!cur->next || cur->next->type != TOKEN_FILE)
			{
				perror(RED"syntax error"DEFAULT);
			}
		}
 
		//heck if a TOKEN_EOF follows after a TOKEN_HEREDOC
		else if (cur->type == TOKEN_HEREDOC)
		{
			if(cur->value == NULL)
			{
				perror(RED"syntax error"DEFAULT);
			}
		}

		//< cmd -l -a 1.txt
		cur = cur->next;
	}
	return 1;
}

//after grouplize calling
bool	is_validate_cmd_block(t_cmd_block *cmd_b)
{
	t_cmd_block *cur;
	int			count;

	count = 0;
	cur = cmd_b;
	while(cur)
	{

		if (cur->args && cur->built_in)
		{
			return false;
		}

		//todo separate this codes
		cur = cur->next;
	}
	if (count == 1)
		return true;
	return false;
}

/*
token -> validate_syntax -> merge to one cmd_block -> validate_cmd_block(atm if theres heredoc excute heredoc)

-> redir set -> check_is_valid_cmd -> excute

tracking child process -> waitpid -> wait4 -> waitpid(WNOHANG)

*/
