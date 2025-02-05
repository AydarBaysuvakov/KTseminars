#include <fcntl.h>
#include <unistd.h>

const int DEFAULT_BUFFER_SIZE = 100;

int main() {
	int fd[2];
	if (pipe(fd)) {
		write(1, "pipe failed\n", 12);
		return 0;
	}

	if (fork()) {
		char buffer[DEFAULT_BUFFER_SIZE] = "";
		int rd = read(fd[0], buffer, DEFAULT_BUFFER_SIZE);
		write(1, buffer, rd);
	}
	else {
		sleep(1);
		write(fd[1], "Hello world\n", 12);
	}
}
