#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

const double _10_9 = 1000000000.0;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Arguments not given.\n");
		return 0;
	}

	struct timespec tp1 = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tp1);
	
	if (fork() == 0) {
		execvp(argv[1], argv + 1);
		return 0;
	}

	wait(NULL);
	struct timespec tp2 = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tp2);
	
	double sec = tp2.tv_sec - tp1.tv_sec + (tp2.tv_nsec - tp1.tv_nsec) / _10_9;
	printf("time is %.3f sec.\n", sec);
}
