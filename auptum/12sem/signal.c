#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

const int BUFFER_SIZE = 1024;

volatile int bit  			= 0;
volatile int wait_info		= 1;
volatile int wait_response 	= 1;

void rd_handler(int signo) {
	wait_response = 0;
}

void wr_handler(int signo) {
	if 		(signo == SIGUSR1)	bit = 0;
    else if (signo == SIGUSR2)	bit = 1;
	wait_info = 0;
}

void reader(pid_t wpid) {
	sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

	struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = rd_handler;
	act.sa_mask = mask;
	sigaction(SIGUSR1, &act, 0);

	char buf[BUFFER_SIZE];
	int  rd;
	while ((rd = read(0, buf, BUFFER_SIZE)) > 0) {
		for (int i = 0; i < rd; ++i) {
            for (int j = 0; j < 8; ++j) {
                bit = (1 << j) & buf[i];
                int sig = (bit == 0) ? SIGUSR1 : SIGUSR2;
                kill(wpid, sig);
        
                wait_response = 1;
                while (wait_response) sigsuspend(&oldmask);
            }
        }
	}
}

void writer(pid_t rpid) {
	sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

	struct sigaction act;
	act.sa_flags 	= 0;
    act.sa_handler 	= wr_handler;
	act.sa_mask 	= mask;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);

	char byte = 1;
    while (byte) {
        byte = 0;
        for (int i = 0; i < 8; i++) {
            wait_info = 1;
            while (wait_info) sigsuspend(&oldmask);
            byte = byte | (bit << i);
            kill(rpid, SIGUSR1);
        }
        write(1, &byte, 1);
    }
}

int main() {
	pid_t parent = getpid();
	pid_t child = fork();
	if (child == 0) {
		reader(parent);
		return 0;
	}
	writer(child);
	wait(NULL);
}
