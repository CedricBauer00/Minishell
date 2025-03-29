"echo Hello | cat | wc"

root@3cefaf3acd65:/#strace -f -e trace=pipe,pipe2,clone,fork,dup2,execve,read,write,openat,close,wait4,waitpid /bin/sh -c "echo hello | cat | wc"
execve("/bin/sh", ["/bin/sh", "-c", "echo hello | cat | wc"], 0x7ffff9bdaf98 /* 8 vars */) = 0
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
close(3)                                = 0

pipe2([3, 4], 0)                        = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLDstrace: Process 148 attached //fork
, child_tidptr=0x7f6a85c9ca10) = 148
[pid   147] close(4)                    = 0 //in parent write pipe.
[pid   148] close(3)                    = 0 //in child printout to next cmd child cant read
[pid   148] dup2(4, 1)                  = 1 // 4 is stdout. in child
[pid   148] close(4)                    = 0 //in child
[pid   148] write(1, "hello\n", 6)      = 6 
[pid   147] pipe2([4, 5], 0)            = 0 //3, 4 , 5
[pid   148] +++ exited with 0 +++

--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=148, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLDstrace: Process 149 attached
, child_tidptr=0x7f6a85c9ca10) = 149 //3,4,5
[pid   147] close(3)                    = 0
[pid   147] close(5 <unfinished ...>
[pid   149] close(4 <unfinished ...>
[pid   147] <... close resumed>)        = 0
[pid   149] <... close resumed>)        = 0
[pid   149] dup2(3, 0)                  = 0 //in child 3 , stdin 
[pid   149] close(3)                    = 0
[pid   149] dup2(5, 1)                  = 1 //in child write of second pipe >> stdout 
[pid   149] close(5 <unfinished ...>
[pid   147] clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD <unfinished ...>
[pid   149] <... close resumed>)        = 0
[pid   149] execve("/usr/bin/cat", ["cat"], 0x55a28ee1e970 /* 8 vars */strace: Process 150 attached
 <unfinished ...>

 
[pid   147] <... clone resumed>, child_tidptr=0x7f6a85c9ca10) = 150
[pid   147] close(4)                    = 0 //in parent close 4
[pid   150] dup2(4, 0 <unfinished ...>
[pid   149] <... execve resumed>)       = 0
[pid   147] close(-1 <unfinished ...>
[pid   150] <... dup2 resumed>)         = 0
[pid   147] <... close resumed>)        = -1 EBADF (Bad file descriptor)
[pid   150] close(4 <unfinished ...>
[pid   147] wait4(-1,  <unfinished ...>
[pid   150] <... close resumed>)        = 0
[pid   147] <... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 148 //memo wait.

[pid   150] execve("/usr/bin/wc", ["wc"], 0x55a28ee1e970 /* 8 vars */ <unfinished ...>
[pid   147] wait4(-1,  <unfinished ...>
[pid   150] <... execve resumed>)       = 0
[pid   149] openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
[pid   149] close(3)                    = 0
[pid   149] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC <unfinished ...>
[pid   150] openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC <unfinished ...>
[pid   149] <... openat resumed>)       = 3
[pid   149] read(3,  <unfinished ...>
[pid   150] <... openat resumed>)       = 3
[pid   149] <... read resumed>"\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
[pid   150] close(3)                    = 0
[pid   150] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
[pid   150] read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
[pid   149] close(3)                    = 0
[pid   150] close(3)                    = 0
[pid   150] read(0,  <unfinished ...>
[pid   149] read(0, "hello\n", 131072)  = 6
[pid   149] write(1, "hello\n", 6 <unfinished ...>
[pid   150] <... read resumed>"hello\n", 16384) = 6
[pid   149] <... write resumed>)        = 6
[pid   149] read(0,  <unfinished ...>
[pid   150] read(0,  <unfinished ...>
[pid   149] <... read resumed>"", 131072) = 0
[pid   149] close(0)                    = 0
[pid   149] close(1 <unfinished ...>
[pid   150] <... read resumed>"", 16384) = 0
[pid   149] <... close resumed>)        = 0
[pid   149] close(2)                    = 0
[pid   150] write(1, "      1       1       6\n", 24      1       1       6
) = 24
[pid   150] close(0 <unfinished ...>
[pid   149] +++ exited with 0 +++
[pid   147] <... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 149 //memo wait
[pid   150] <... close resumed>)        = 0
[pid   147] --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=149, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---
[pid   150] close(1 <unfinished ...>
[pid   147] wait4(-1,  <unfinished ...>
[pid   150] <... close resumed>)        = 0
[pid   150] close(2)                    = 0
[pid   150] +++ exited with 0 +++

<... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 150 //memo wait 

--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=150, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---
wait4(-1, 0x7ffc3690578c, WNOHANG, NULL) = -1 ECHILD (No child processes)
+++ exited with 0 +++

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int echo_Hello_pipe_cat_pipe_wc()
{
	int fd[2];
	pid_t pid;
	int status;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], stdout);
		close(fd[1]);
		//find echo excute echo
		write(1, "hello\n", 6);
		exit(0);
	}
	//parent
	else
	{
		close(fd[1]);
	}
	waitpid(pid, &status, 0);

	int fd1[2];
	pipe(fd1);
	pid	= fork();
	if (pid == 0)
	{
		dup2(fd[0], stdin);
		close(fd[0]);
		close(fd1[0]);
		dup2(fd1[1], stdout);
		close(fd1[1]);
		execve("/usr/bin/cat", "cat");
		exit(0);
	}
	else
	{
		close(fd[0]);
		close(fd1[1]);
	}
	waitpid(pid, status, 0);

	pid = fork();
	if (pid == 0)
	{
		dup2(fd1[0], stdin);
		close(fd1[0]);
		execve("/usr/bin/wc", "wc");
	}
	else
	{
		close(fd1[0]);
		waitpid(pid, &status,0);
	}
	
	//to check 
	while (waitpid(-1, NULL, WNOHANG) > 0);
}
