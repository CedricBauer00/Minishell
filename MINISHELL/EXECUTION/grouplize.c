#include "../INCLUDE/main.h"

/*
	1. 명령어가 빌트인이냐 외부 명령어냐?
	2. 외부명령어면 포크 아니면 그냥 부모에서
	3. 외부명령어면 fd를 모두 자식에서 셋팅 
	4. 빌트인이면 fd를 모두 부모에서 셋팅
*/

//Divided by command
void grouplize(t_token *token, t_gc_list *gc_lst)
{
	if (!token)
		return ;
	
	while (token != NULL)
	{
		merge_to_one_cmd(&token, gc_lst);
		if(token && token->type == TOKEN_PIPE)
			token = token->next;
		if (token == NULL)
			break;
	}
}

void	merge_to_one_cmd(t_token **token, t_gc_list *gc_lst)
{
	t_command_node *new_c_node;
	t_token *cur;
	
	cur = *token;
	new_c_node = init_command_struct();
	is_exited((t_command_node*)new_c_node, gc_lst);
	while(cur && cur->type != TOKEN_PIPE)
	{
		if (cur && cur->type == TOKEN_CMD)
		{
			new_c_node->cmd = cur->value;
		}
		if (cur && cur->type == TOKEN_FLAGS)
		{
			//todo : new_c_node->args do_alloc
			int i = 0;
			while(cur && cur->type == TOKEN_FLAGS)
			{
				new_c_node->args[i] = cur->value;
				i++;
				cur = cur->next;
			}
			new_c_node->args[i] = NULL;
		}
		if (cur && cur->type == TOKEN_BUILT_IN)
		{
			new_c_node->built_in = cur->value;
		}
		if (cur && cur->type == TOKEN_REDIRECT_IN)  //token->value : < filename
		{
			if(cur->next && cur->next->type == TOKEN_WORD)
			{
				new_c_node->infile_name = cur->next->value;
				if (cur->next->next)
					cur = cur->next->next;
				continue;
			}
			else
			{
				fprintf(stderr,"syntax error near unexpected token `newline'");
			}
		}
		if (cur && cur->type == TOKEN_REDIRECT_OUT) //token->value : >
		{
			if(cur->next && cur->next->type == TOKEN_WORD)
			{
				new_c_node->outfile_name = cur->next->value;
				if (cur->next->next)
					cur = cur->next->next; 
				continue;
			}
			else
			{
				fprintf(stderr,"syntax error near unexpected token `newline'");
			}
		}
		cur = cur->next;
	}
	*token = cur;
	//todo 연결리스트로 연결 !
}

//redirection
//ok cmd < filename < fileanme
//ok cmd > filename > filename > filename
//ok < 1.txt cat 3.txt 이경우 작동은되는데 cat 3.txt만
//ok cmd < input.txt > output.txt
//ok cmd filename > fileanme fileanme filename
