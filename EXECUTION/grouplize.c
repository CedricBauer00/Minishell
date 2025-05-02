#include "execution.h"

/*
	1. 명령어가 빌트인이냐 외부 명령어냐?
	2. 외부명령어면 포크 아니면 그냥 부모에서
	3. 외부명령어면 fd를 모두 자식에서 셋팅 
	4. 빌트인이면 fd를 모두 부모에서 셋팅
*/

//Divided by command
void grouplize(t_token *token, t_cmd_block **cmd_block, t_gc *gc)
{
	if (!token)
		return ;
	while (token != NULL)
	{
		if(token && token->type == TOKEN_PIPE)
			token = token->next;
		t_cmd_block *new_cmd_block = merge_to_one_cmd(&token, gc);
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


t_cmd_block	*merge_to_one_cmd(t_token **token, t_gc *gc)
{
	t_cmd_block *new_cmd_block;
	t_io_streams_list *new_io_streams;
	t_token *cur;
	int	i;
	
	i = 0;
	cur = *token;
	t_token *temp = cur;
	new_cmd_block = init_command_struct(gc->temp);
	is_exited((t_cmd_block*)new_cmd_block, gc);
	while(temp && temp->type != TOKEN_PIPE)
	{
		if (temp && temp->type == TOKEN_ARG)
		{
			i++;
		}
		temp = temp ->next;
	}
	new_cmd_block->args = (char**)do_alloc(&gc->temp, sizeof(char*) * (i + 1), TYPE_DOUBLE_PTR, "new_cmd_block->args");
	is_exited(new_cmd_block->args, gc);
	i = 0;
	while(cur && cur->type != TOKEN_PIPE) //[cat] [-e] [hello] [hello] [|]
	{
		if (cur && cur->type == TOKEN_BUILT_IN)
		{
			new_cmd_block->is_built_in = true;
			new_cmd_block->built_in = gc_strdup(cur->value, &gc->temp);
			//new_cmd_block->args[i++] = gc_strdup(cur->value, gc->temp);
		}

		if (cur && cur->type == TOKEN_ARG)
		{
			new_cmd_block->args[i++] = gc_strdup(cur->value, &gc->temp);
		}
		
		if (cur && (cur->type & (TOKEN_REDIRECT_IN | TOKEN_REDIRECT_OUT | TOKEN_APPEND | TOKEN_HEREDOC)))  //token->value : < filename
		{
			new_io_streams = init_io_stream_struct(gc->temp);
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
			if(cur->type & (TOKEN_HEREDOC))
			{
				fprintf(stderr, RED"if heredoc in grouplize()\n"DEFAULT);
				//todo i did it for the test later delete
				new_io_streams->heredoc_eof = gc_strdup("eof", &gc->temp);
					// new_io_streams->heredoc_eof = gc_strdup(cur->value, gc_lst);
			}
			else if(cur->next && cur->next->type == TOKEN_FILE)
			{
				if (cur->type == TOKEN_REDIRECT_IN)
					new_io_streams->infile_name = gc_strdup(cur->next->value, &gc->temp);
				else if (cur->type == TOKEN_REDIRECT_OUT)
					new_io_streams->outfile_name = gc_strdup(cur->next->value, &gc->temp);
				else if (cur->type == TOKEN_APPEND)
					new_io_streams->append_file_name = gc_strdup(cur->next->value, &gc->temp);
				cur = cur->next;
				continue;
			}
			else
			{
				fprintf(stderr, "syntax error near unexpected token `newline'");
			}
		}
		new_cmd_block->args[i] = NULL;
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


