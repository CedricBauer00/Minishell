"echo hello | cat"

root@5c5789affe38:/#strace -f -e trace=pipe,pipe2,clone,fork,dup2,execve,read,write,openat,close /bin/sh -c "echo hello | cat"
execve("/bin/sh", ["/bin/sh", "-c", "echo hello | cat"], 0x7ffde9cef058 /* 7 vars */) = 0
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
close(3)                                = 0
pipe2([3, 4], 0)                        = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLDstrace: Process 152 attached
, child_tidptr=0x7faa1d1dca10) = 152

//parent
/*
	부모 : 151
	자식 : 152
	부모는 파이프 쓰기끝을 닫고
	
	in child process.
	자식은 파이프 읽기끝을 닫는다.
	그리고 STDOUT_FILENO을 파이프 쓰기끝으로 리디렉션
	그리고 파이프쓰기끝을 닫는다.
	그리고 hello 를쓴다.
	그리고 exit(0)을한다.
	그리고 SIGCHLD를 한다.
*/

[pid   151] close(4 <unfinished ...>
[pid   152] close(3)                    = 0
[pid   151] <... close resumed>)        = 0
[pid   152] dup2(4, 1)                  = 1
[pid   152] close(4)                    = 0
[pid   152] write(1, "hello\n", 6)      = 6
[pid   152] +++ exited with 0 +++
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=152, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---


clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLDstrace: Process 153 attached
, child_tidptr=0x7faa1d1dca10) = 153
[pid   151] close(3)                    = 0
[pid   153] dup2(3, 0 <unfinished ...>
[pid   151] close(-1 <unfinished ...>
[pid   153] <... dup2 resumed>)         = 0
[pid   151] <... close resumed>)        = -1 EBADF (Bad file descriptor)
[pid   153] close(3)                    = 0
[pid   153] execve("/usr/bin/cat", ["cat"], 0x5635b9951918 /* 7 vars */) = 0
[pid   153] openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
[pid   153] close(3)                    = 0
[pid   153] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
[pid   153] read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
[pid   153] close(3)                    = 0
[pid   153] read(0, "hello\n", 131072)  = 6
[pid   153] write(1, "hello\n", 6hello
)      = 6
[pid   153] read(0, "", 131072)         = 0
[pid   153] close(0)                    = 0
[pid   153] close(1)                    = 0
[pid   153] close(2)                    = 0
[pid   153] +++ exited with 0 +++
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=153, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---
+++ exited with 0 +++


