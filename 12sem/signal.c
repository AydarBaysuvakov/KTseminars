#include <signal.h>
#include <stdio.h>

char string[] = "Hello world";

int main() {
	struct sigaction act;
	if (fork() == 0) {
		while (1) {
			sigsuspend(&act.sa_mask);
			putchar(*((char*) &act.sa_mask));
			sigaction(SIGUSR1, &act, 0);
		}
	}
	for (int i = 0; i < 12, i++) {
		sigemptyset(&act.sa_mask);
		/* Set mask with letter */
		sigaction(SIGUSR1, &act, 0);
		sigsuspend(&act.sa_mask);
	}
}
