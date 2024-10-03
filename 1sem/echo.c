#include <unistd.h>
#include <stdio.h>
#include <string.h>

static void write_error() {
	perror("write error\n");
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		return 0;
	}

	int nkey = 0, argn = 1;
	if (argv[argn][0] == '-') {
		switch (argv[argn][1]) {
			case 'n': {nkey = 1; ++argn;}
		}
	}

	for (; argn < argc; ++argn) {
		if (write(1, argv[argn], strlen(argv[argn])) < 0) write_error();
		if (argn + 1 != argc && write(1, " ", 1) < 0) write_error();
	}

	if (!nkey && write(1, "\n", 1) < 0) write_error();
}
