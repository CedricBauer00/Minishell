#include "../INCLUDE/main.h"

/*
	1. 명령어가 빌트인이냐 외부 명령어냐?
	2. 외부명령어면 포크 아니면 그냥 부모에서
	3. 외부명령어면 fd를 모두 자식에서 셋팅 
	4. 빌트인이면 fd를 모두 부모에서 셋팅
*/

//Divided by command
void grouplize(t_token *token, t_cmd_block **cmd_block, t_gc_list *gc_lst)
{
	if (!token)
		return ;
	while (token != NULL)
	{
		if(token && token->type == TOKEN_PIPE)
			token = token->next;
		t_cmd_block *new_cmd_block = merge_to_one_cmd(&token, gc_lst);
		if (*cmd_block == NULL)
		{
			*cmd_block = new_cmd_block;
		}
		else
		{
			t_cmd_block *last = *cmd_block;
			while(last->next)
			{
				last = last->next;
			}
			last->next = new_cmd_block;
			new_cmd_block->prev = last;
		}
		// if(token == NULL)
		// 	break;
	}
}

//think aobut that how can we distinguish cmd and word
//todo do not assign just address otherwise use strdup for makking cmd_block list's node.
t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc_list *gc_lst)
{
	t_cmd_block *new_cmd_block;
	t_io_streams_list *new_io_streams;
	t_token *cur;
	
	cur = *token;
	new_cmd_block = init_command_struct(gc_lst);
	is_exited((t_cmd_block*)new_cmd_block, gc_lst);
	while(cur && cur->type != TOKEN_PIPE)
	{
		//if token_args
		// cat -e -l -a
		if (cur && cur->type == TOKEN_ARGS)
		{
			new_cmd_block->args = gc_strdup(cur->value, gc_lst);
		}
		
		//TOKEN_ARGS = cmd -a -l -k filename

		if (cur && cur->type == TOKEN_BUILT_IN)
		{
			new_cmd_block->built_in = gc_strdup(cur->value, gc_lst);
		}

		if (cur && (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND)))  //token->value : < filename
		{
			new_io_streams = init_io_stream_struct(gc_lst);
			if(!new_cmd_block->io_streams)
				new_cmd_block->io_streams = new_io_streams;
			else
			{
				t_io_streams_list *last;
				last = new_cmd_block->io_streams;
				while(last->next)
				{
					last = last->next;
				}
				last->next = new_io_streams;
			}
			if(cur->next && cur->next->type == TOKEN_FILE)
			{
				if (cur->type == TOKEN_REDIRECT_IN)
					new_io_streams->infile_name = gc_strdup(cur->next->value, gc_lst);
				else if (cur->type == TOKEN_REDIRECT_OUT || cur->type == TOKEN_APPEND)
					new_io_streams->outfile_name = gc_strdup(cur->next->value, gc_lst);
				cur = cur->next;
				continue;
			}
			else if(cur->prev && cur->prev->type == TOKEN_FILE) // -a filename 
			{
				if (cur->type == TOKEN_REDIRECT_IN)
					new_io_streams->infile_name = gc_strdup(cur->prev->value, gc_lst);
				else if(cur->type == TOKEN_REDIRECT_OUT || cur->type == TOKEN_APPEND)
					new_io_streams->outfile_name = gc_strdup(cur->prev->value, gc_lst);
				else if (cur->type == TOKEN_HEREDOC)
				{
					if (cur->prev->type == TOKEN_ARGS)
					{
						new_cmd_block->args = gc_strdup(cur->prev->value, gc_lst);
					}
					new_io_streams->heredoc_eof = gc_strdup(cur->value, gc_lst);
				}
			}
			else
			{
				fprintf(stderr, "syntax error near unexpected token `newline'");
			}
		}
		cur = cur->next;
	}
	*token = cur;
	return new_cmd_block;
}




//redirection
//ok cmd < filename < fileanme
//ok cmd > filename > filename > filename
//ok < 1.txt cat 3.txt 이경우 작동은되는데 cat 3.txt만
//ok cmd < input.txt > output.txt
//ok cmd filename > fileanme fileanme filename


