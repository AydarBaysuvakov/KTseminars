#include <stdio.h>
#include <dirent.h>
#include <errno.h>

const int STRING_LEN = 1024;

void list(const char* path, int attaching) {
    DIR* dir;
	if (!(dir = opendir(path))) {
		perror("Can't open directory");
		return;
	}

    char filepath[STRING_LEN];
	struct dirent *ent;
	while (ent = readdir(dir)) {
        if (ent->d_name[0] != '.') {
            for (int i = 0; i < attaching; i++) putchar('\t');	    	
            printf("%s\n", ent->d_name);
            sprintf(filepath, "%s/%s", path, ent->d_name);
            if (ent->d_type == DT_DIR) list(filepath, attaching + 1);
        }
	}
	if (errno) {
		perror("dirent error");
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Arguments not given\n");
		return 0;
	}

	list(argv[1], 0);
}
