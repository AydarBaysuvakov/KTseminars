#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>

volatile int flag = 1;
volatile int draw = 1;
struct winsize ws = {0, 0};

void handler(int signo) {
	if (signo == SIGINT) { flag = 0; }
	if (signo == SIGWINCH) {
		ioctl(1, TIOCGWINSZ, &ws);
	        draw = 1;
	}	       
}

int main() {
	signal(SIGWINCH, handler);
	signal(SIGINT, handler);
	ioctl(1, TIOCGWINSZ, &ws);
	while (flag) {
		if (draw) {
			printf("\e[H\e[J");
			printf("\e[%d;%dH", (ws.ws_row) / 2, (ws.ws_col - 12) / 2);
			// printf("%d %d\n", ws.ws_row, ws.ws_col);
			printf("Hello world\n");
			draw = 0;
		}
	}
	printf("\e[H\e[J");
	printf("Goodbye\n");
}
