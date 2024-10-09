#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>

const int DEFAULT_BUFFER_SIZE = 10000;
const double _10_9 = 1000000000.0;

void count_lwb(char* string, int* line, int* word, int byte, int* word_flag) {
	for (int i = 0; i < byte; i++) {
		if (string[i] == '\n') (*line)++;

		if (string[i] == '\n' || string[i] == ' ' || string[i] == '\t') *word_flag = 1;
		else if (*word_flag) {
			(*word)++;
			*word_flag = 0;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		write(1, "Arguments not given\n", 20);
		return 0;
	}

	int fd[2];
	if (pipe(fd) == -1) {
		write(2, "pipe failed\n", 12);
		return 0;
	}

    	char buffer[DEFAULT_BUFFER_SIZE] = "";
	int line = 0, word = 0, byte = 0;
	int rt, word_flag = 1;

	struct timespec tp1 = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tp1);

	if (fork() == 0) {
        close(fd[0]);
		if (dup2(fd[1], 1) < 0) {
			write(2, "dup failed\n", 11);
			return 0;
		}
		execvp(argv[1], argv + 1);
		return 0;
	}
    	close(fd[1]);
    	while ((rt = read(fd[0], buffer, DEFAULT_BUFFER_SIZE)) > 0) {
		count_lwb(buffer, &line, &word, rt, &word_flag);
		byte += rt;
	}

	struct timespec tp2 = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tp2);

	double sec = tp2.tv_sec - tp1.tv_sec + (tp2.tv_nsec - tp1.tv_nsec) / _10_9;
	printf("time is %.3f sec.\n", sec);
	printf("%6d\t%6d\t%6d\n", line, word, byte);
}
