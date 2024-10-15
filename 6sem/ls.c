#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int main(const int argc, char* argv[]) {
    	const char* files[argc] = {NULL};
	char lflag = 0;
	char iflag = 0;
	// TODO: bit field (union)
	if (argc < 2) {
		files[0] = ".";
	}
    	else { 
		int file_found = 0;
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				for (int j = 1; argv[i][j]; j++) {
					switch (argv[i][j]) {
						case 'l': lflag = 1; break;
						case 'i': iflag = 1; break;
						default: printf("Wrong flag: %c\n", argv[i][j]);
					}
				}
			}
			else {
				files[i] = argv[i];
				file_found = 1;
			}
		}
		if (!file_found) files[0] = ".";

	}

    	for (int i = 0; i < argc; i++) {
		if (files[i]) {
	    		DIR* dir;
		    	if (!(dir = opendir(files[i]))) {
			   	perror("Can't open directory");
		    		continue;
		    	}
	
			printf("%s:\n", files[i]);
			struct dirent *ent;
			while (ent = readdir(dir)) {
				if (iflag) printf("%ld ", ent->d_ino);
				if (lflag) {
					printf("%c%c%c%c%c%c%c%c%c ", '-', '-', '-', '-', '-', '-', '-', '-', '-');
					printf("(pid) (uid) (gid) ");
					printf("(size) (data) %s\n", ent->d_name);
				}
				else printf("%s ", ent->d_name);
			}
			if (errno) {
				perror("dirent error");
			}
			closedir(dir);
			putchar('\n');
		}
	}
}
