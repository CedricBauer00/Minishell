# include "../INCLUDE/main.h"
/* readline함수를 사용하기위한 헤더 */
# include <readline/readline.h>

/* add_history함수를 사용하기위한 헤더 */
# include <readline/history.h>

/* printf함수를 사용하기위한 헤더 */
// # include <stdio.h>

// /* free함수를 사용하기위한 헤더 */
// # include <stdlib.h>

/*

*/
//termianl path 

/*
2. heredoc 입력 처리기
delimiter가 나오기 전까지 사용자 입력을 반복해서 받아야 함.

예: readline("> ") 사용
//ok!

받은 내용을:
임시 파일(tmpfile)에 저장해야 해.

중요한 건 fork 전에 미리 처리해두는 것.

3. 입력 저장 방식
작은 heredoc 내용은 pipe()를 쓰는 게 좋아.

내용이 크거나, 파일처럼 seek이 필요한 경우는 mkstemp() 등으로 실제 임시파일을 만들어야 해.

4. 입력 리디렉션 (dup2)
heredoc 내용을 저장한 파이프나 파일을 stdin으로 연결해야 함.

c
복사
편집
dup2(pipe_read_fd, STDIN_FILENO);
5. 프로세스 구조
heredoc 처리는 보통 명령 실행 전, 부모 프로세스에서 처리함.

이렇게 해야 fork 후 자식에서 리디렉션만 하고 바로 exec할 수 있어.

*/
//fd sollte einfach STDIN_FILENO
// char *get_terminal_path(int fd)
// {
// 	char *tty_path;
	
// 	if (fd < 0)
// 		return (NULL);
// 	tty_path = ttyname(fd);
// 	if (!tty_path)
// 		return (NULL);
// 	return (tty_path);
// }

//todo think about that mmap how it does work!
int	process_heredoc(t_io_streams_list *io_streams)
{
	int	fd_heredoc;
	
	fd_heredoc = 0;
	while (io_streams->heredoc_eof)
	{
		//char filename[64];
		//todo make multiples files;;;
		//filename = "temp_heredoc";
		//if multiples heredoc we need multiples filenames..

		fd_heredoc = open("temp_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_heredoc == -1)
		{
			perror(RED"failed to open temp_heredoc"DEFAULT);
			return -1;
		}
		while(1)
		{
			char *line;

			line = readline("> ");
			if (!line || strcmp(line, "eof") == 0)
			{
				free(line);
				break;
			}
			write(fd_heredoc, line, strlen(line));
			write(fd_heredoc, "\n", 1);
			free(line);
		}
		io_streams->heredoc_fd = fd_heredoc;
		close(fd_heredoc);
		io_streams = io_streams->next;
	}

	// //todo think where should i recover it 
	// if (dup2(fd_org_read, STDOUT_FILENO) == -1)
	// {
	// 	close(fd_org_read);
	// 	return -1;
	// }
	// close(fd_org_read);
	return 1;
}


//bei der durchfuehrung des cmd muss ich den FD des temporäre Datei umleiten.
// int main()
// {
// 	heredoc();
// }