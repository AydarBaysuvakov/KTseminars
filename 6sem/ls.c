#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

const int STRING_LEN = 1024;

enum RIGHTS {
	X_OTHER 	= 1 << 0,
	W_OTHER 	= 1 << 1,
	R_OTHER 	= 1 << 2,
	X_GROUP 	= 1 << 3,
	W_GROUP 	= 1 << 4,
	R_GROUP 	= 1 << 5,
	X_OWNER 	= 1 << 6,
	W_OWNER 	= 1 << 7,
	R_OWNER 	= 1 << 8,
	DIRECTORY 	= 1 << 14,
	REGULAR   	= 1 << 15
};

// -l --long, -i --inode, -R --recursive, -a --all, -d --directory

// TODO: time, blocksize

enum FLAGS {
	long_flag 		= 1 << 0,
	inode_flag 		= 1 << 1,
	recursive_flag 	= 1 << 2,
	all_flag 		= 1 << 3,
	directory_flag 	= 1 << 4,
};

void print_inode(struct dirent* ent, int flags) {
    if (flags & inode_flag)
        printf("%llu ", (long long int) ent->d_ino);
}


void print_listing(struct stat *st, int flags) {
    if(flags & long_flag){
        if (st->st_mode & DIRECTORY) printf("d");
		if (st->st_mode & REGULAR  ) printf("-");
		
		printf((st->st_mode & R_OWNER) ? "r" : "-");
		printf((st->st_mode & W_OWNER) ? "w" : "-");
		printf((st->st_mode & X_OWNER) ? "x" : "-");

		printf((st->st_mode & R_GROUP) ? "r" : "-");
		printf((st->st_mode & W_GROUP) ? "w" : "-");
		printf((st->st_mode & X_GROUP) ? "x" : "-");

		printf((st->st_mode & R_OTHER) ? "r" : "-");
		printf((st->st_mode & W_OTHER) ? "w" : "-");
		printf((st->st_mode & X_OTHER) ? "x" : "-");

        printf(" %3lld ", (long long int) st->st_nlink); 
        printf("%s ", getpwuid(st->st_uid)->pw_name);
        printf("%s", getgrgid(st->st_gid)->gr_name);
		printf("%12lld ",  (long long int) st->st_blksize * st->st_blocks / 8);

        char buffer[STRING_LEN];
        strftime(buffer, STRING_LEN, "%D %T", gmtime(&st->st_mtime));
        printf("%s ", buffer);
    }
}

const char* getname(const char *path) {
	const char* ptr = path;
	const char* filename = path;
	while (*ptr) {
		if (*ptr == '/' && *(ptr + 1))
			filename = ptr + 1;
		ptr++;
	}
	return filename;
}

void print_directory(const char* path, int flags) {
    struct stat    st;
    struct dirent* ent;
    char filepath[STRING_LEN];
	DIR* dir = opendir(".");
	
	const char* filename = getname(path);
	int backslash = filename[strlen(filename) - 1] == '/' ? 1 : 0;

    while (ent = readdir(dir)) {
		if (!strncmp(filename, ent->d_name, strlen(filename) - backslash)) {
			sprintf(filepath, "./%s", ent->d_name);
			stat(filepath, &st);
			print_inode(ent  , flags); 
			print_listing(&st, flags);
			printf(" %s ", ent->d_name);
		}
	}
	closedir(dir);
}

void print_default(const char* path, int flags) {
    struct stat    st;
    struct dirent* ent;
    char filepath[STRING_LEN];

	DIR* dir = opendir(path);
	if (!dir) {
		perror("Can't open directory");
		return;
	}

	printf("%s:\n", path);
	while (ent = readdir(dir)) {
		if ((ent->d_name[0] != '.') || (flags & all_flag)) {
			sprintf(filepath, "%s/%s", path, ent->d_name);
			stat(filepath, &st);
			print_inode(ent  , flags);
			print_listing(&st, flags);
			printf("%s  ", ent->d_name);
			if (flags & long_flag) putchar('\n');
		}
	}
	if (flags & recursive_flag) {
		rewinddir(dir);
		while (ent = readdir(dir)) {
			if (ent->d_type == DT_DIR && ent->d_name[0] != '.') {
				sprintf(filepath, "%s/%s", path, ent->d_name);
				putchar('\n');
				print_default(filepath, flags);
			}
		}
	}
	putchar('\n');
	closedir(dir);
}

void print_dirs(const char* path, int flags) {
    if (flags & directory_flag) {
		print_directory(path, flags);
    }
	else {
		print_default(path, flags);
	}
}

static struct option longopts[] = { 
	{ "long"     , 	no_argument, NULL, 'l'},
	{ "inode"    , 	no_argument, NULL, 'i'},
	{ "recursive", 	no_argument, NULL, 'R'},
	{ "all"      , 	no_argument, NULL, 'a'},
	{ "directory", 	no_argument, NULL, 'd'},
};

int main(int argc, char* argv[]) {
	int opt, flags = 0;
    while ((opt = getopt_long(argc, argv, "liRad", longopts, NULL)) != -1) {
        switch (opt) {
            case 'l': flags = flags | long_flag;  		break; 
            case 'i': flags = flags | inode_flag; 		break; 
            case 'R': flags = flags | recursive_flag; 	break;
            case 'a': flags = flags | all_flag; 		break; 
            case 'd': flags = flags | directory_flag; 	break;
        }
    }

	if (optind == argc) {
        print_dirs(".", flags); 
        printf("\n");
    }
    else {
        for (int i = optind; i < argc; i++) {
            print_dirs(argv[i], flags);
            printf("\n");
        }
    }
}
