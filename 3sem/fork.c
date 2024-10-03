#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("Program started. pid - %d\n", getpid());

	for (int i = 0; i < 5; ++i) {
		if (fork() == 0) {
			printf("curent pid: %d; parent pid: %d\n", getpid(), getppid());
			return 0;
			}
		sleep(1);
	}
}
