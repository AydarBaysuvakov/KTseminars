#include <stdio.h>
#include <sys/types.h>

int main() {
	pid_t proc_pid = getpid();
       	pid_t par_pid  = getppid();
	printf("curent pid: %d\nparent: %d\n", proc_pid, par_pid);	
}
