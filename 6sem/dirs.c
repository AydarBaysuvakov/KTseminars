#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Arguments not given\n");
		return 0;
	}

	DIR* dir;
	if (!(dir = opendir(argv[1]))) {
		perror("Can't open directory");
		return 0;
	}

	struct dirent *ent;
	while (ent = readdir(dir)) {
		printf("i_node: %10ld type: %4d name: %s\n", ent->d_ino, ent->d_type, ent->d_name);
	}
	if (errno) {
		perror("dirent error");
	}
	closedir(dir);
}
