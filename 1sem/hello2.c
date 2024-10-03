#include <stdio.h>
#include <unistd.h>

int main() {
	if (write(1, "hello world\n", 12) < 0) {
		perror("write failed\n");
	}
}
