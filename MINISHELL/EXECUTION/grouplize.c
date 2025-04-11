#include "../INCLUDE/main.h"

/*
	1. 명령어가 빌트인이냐 외부 명령어냐?
	2. 외부명령어면 포크 아니면 그냥 부모에서
	3. 외부명령어면 fd를 모두 자식에서 셋팅 
	4. 빌트인이면 fd를 모두 부모에서 셋팅
*/

//Divided by command
void grouplize(t_token *token, t_cmd_block *cmd_block, t_gc_list *gc_lst)
{
	if (!token)
		return ;
	
	while (token != NULL)
	{
		merge_to_one_cmd(&token, &cmd_block, gc_lst);
		if(token && token->type == TOKEN_PIPE)
			token = token->next;
		if (token == NULL)
			break;
	}
}

//todo do not assign just address otherwise use strdup for makking cmd_block list's node.
void	merge_to_one_cmd(t_token **token, t_cmd_block **cmd_block, t_gc_list *gc_lst)
{
	t_cmd_block *new_cmd_block;
	t_io_streams_list *new_io_streams;
	t_token *cur;
	
	cur = *token;
	new_cmd_block = init_command_struct(gc_lst);
	is_exited((t_cmd_block*)new_cmd_block, gc_lst);
	if (!(*cmd_block))
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
		last -> next = new_cmd_block;
	}

	while(cur && cur->type != TOKEN_PIPE)
	{
		if (cur && cur->type == TOKEN_CMD)
		{
			new_cmd_block->cmd = gc_strdup(cur->value, gc_lst);
		}
		if (cur && cur->type == TOKEN_FLAGS)
		{
			int count = 0;
			t_token *temp = cur;
			while (temp && temp->type == TOKEN_FLAGS)
			{
				count++;
				temp = temp->next;
			}
			new_cmd_block->args = do_alloc(gc_lst, count + 1, TYPE_DOUBLE_PTR);
			int i = 0;
			while(cur && cur->type == TOKEN_FLAGS)
			{
				new_cmd_block->args[i] = gc_strdup(cur->value, gc_lst);
				i++;
				cur = cur->next;
			}
			new_cmd_block->args[i] = NULL;
		}

		if (cur && cur->type == TOKEN_BUILT_IN)
		{
			new_cmd_block->built_in = gc_strdup(cur->value, gc_lst);
		}

		if (cur && (cur->type == TOKEN_REDIRECT_IN || cur->type == TOKEN_REDIRECT_OUT || cur->type == TOKEN_APPEND))  //token->value : < filename
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
			if(cur->next && cur->next->type == TOKEN_WORD)
			{
				if (cur->type == TOKEN_REDIRECT_IN)
					new_cmd_block->io_streams->infile_name = gc_strdup(cur->next->value, gc_lst);
				else if (cur->type == TOKEN_REDIRECT_OUT)
					new_cmd_block->io_streams->outfile_name = gc_strdup(cur->next->value, gc_lst);
				else if (cur->type == TOKEN_APPEND)
					new_cmd_block->io_streams->outfile_name = gc_strdup(cur->next->value, gc_lst);
			
				cur = cur->next;
				continue;
			}
			else
			{
				fprintf(stderr, "syntax error near unexpected token `newline'");
			}
		}

		if (cur && cur->type == TOKEN_HEREDOC)
		{
			int heredoc_result;
			heredoc_result = heredoc();
			if (heredoc_result == -1)
			{
				//todo handle error case!
			}
		}
		cur = cur->next;
	}
	*token = cur;
}



//redirection
//ok cmd < filename < fileanme
//ok cmd > filename > filename > filename
//ok < 1.txt cat 3.txt 이경우 작동은되는데 cat 3.txt만
//ok cmd < input.txt > output.txt
//ok cmd filename > fileanme fileanme filename
