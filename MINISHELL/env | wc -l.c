219   execve("/bin/sh", ["/bin/sh", "-c", "env | wc -l"]
219   openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
219   close(3)                          = 0
219   openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
219   read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
219   close(3)                          = 0


219   pipe2([3, 4], 0)                  = 0 //memo pipe 
219   clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f00f5e61a10) = 220 //memo fork
219   close(4)                          = 0
220   close(3)                          = 0
220   dup2(4, 1)                        = 1
219   clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD <unfinished ...>
220   close(4)                          = 0
219   <... clone resumed>, child_tidptr=0x7f00f5e61a10) = 221
220   execve("/usr/bin/env", ["env"], ["HOSTNAME=796eb7844e49", "SHLVL=1", "HOME=/root", "_=/usr/bin/strace", "TERM=xterm", "PATH=/usr/local/sbin:/usr/local/"..., "PWD=/"] <unfinished ...>
219   close(3)                          = 0
219   close(-1 <unfinished ...>
221   dup2(3, 0 <unfinished ...>
219   <... close resumed>)              = -1 EBADF (Bad file descriptor)
221   <... dup2 resumed>)               = 0
219   wait4(-1,  <unfinished ...>
220   <... execve resumed>)             = 0
221   close(3)                          = 0
221   execve("/usr/bin/wc", ["wc", "-l"], ["HOSTNAME=796eb7844e49", "SHLVL=1", "HOME=/root", "_=/usr/bin/strace", "TERM=xterm", "PATH=/usr/local/sbin:/usr/local/"..., "PWD=/"]) = 0
220   openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
221   openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
220   close(3)                          = 0
220   openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
221   close(3 <unfinished ...>
220   read(3,  <unfinished ...>
221   <... close resumed>)              = 0
220   <... read resumed>"\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
221   openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
221   read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20t\2\0\0\0\0\0"..., 832) = 832
220   close(3)                          = 0
221   close(3)                          = 0
221   read(0,  <unfinished ...>
220   write(1, "HOSTNAME=796eb7844e49\nSHLVL=1\nHO"..., 142 <unfinished ...>
221   <... read resumed>"HOSTNAME=796eb7844e49\nSHLVL=1\nHO"..., 16320) = 142
220   <... write resumed>)              = 142
221   read(0,  <unfinished ...>
220   close(1 <unfinished ...>
221   <... read resumed>"", 16320)      = 0
220   <... close resumed>)              = 0
220   close(2)                          = 0
221   write(1, "7\n", 2)                = 2
221   close(0)                          = 0
220   +++ exited with 0 +++
219   <... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 220
221   close(1 <unfinished ...>
219   --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=220, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---
221   <... close resumed>)              = 0
221   close(2)                          = 0
219   wait4(-1,  <unfinished ...>
221   +++ exited with 0 +++
219   <... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 221
219   --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=221, si_uid=0, si_status=0, si_utime=0, si_stime=0} ---
219   wait4(-1, 0x7ffc9dddea7c, WNOHANG, NULL) = -1 ECHILD (No child processes)
219   +++ exited with 0 +++