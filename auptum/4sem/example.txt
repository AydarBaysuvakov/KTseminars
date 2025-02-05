#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int DEFAULT_BUFFER_SIZE = 64;

int in_to_out(int in, int out) {
	char buffer[DEFAULT_BUFFER_SIZE] = {0};
	int rd, wr;
	while ((rd = read(in, buffer, DEFAULT_BUFFER_SIZE)) > 0) {
		while ((wr = write(out, buffer, rd)) > 0) rd -= wr;
		if (wr < 0) {
			perror("Write failed");
			return 0;
		}
	}
	if (rd < 0) {
		perror("Read failed");
		return 0;
	}
	return 1;
}

int main(int argc, char* argv[]) {
	int in = 0, out = 1;
	
	if (argc == 1) {
		in_to_out(in, out);
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		int std = 0;
		if (argv[i][0] == '-' && argv[i][1] == '\0') {
			in = 0;
			std = 1;
		}
		else {
			in = open(argv[i], O_RDONLY);
			if (in < 0) {
				perror("Input file open failed");
				return 0;
			}
		}
		in_to_out(in, out);
		if (!std) {
			if (close(in) < 0) {
				perror("File close failed");
				return 0;
			}
		}
	}
}
