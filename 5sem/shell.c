#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

const size_t BUFFER_SIZE = 1024;

struct Buf {
	char* buffer;
	size_t buf_size;
	size_t len;
	char** parsed;
	size_t par_size;
	char*** programs;
	size_t program_n;
	int signal;
};

void parse(struct Buf* buf) {
	int flag = 1; 
	buf->par_size = 0;
	buf->program_n = 1;
	for (int i = 0; i < buf->len; i++) {
		if (buf->buffer[i] == '|') buf->program_n++;
		if (buf->buffer[i] == ' ') flag = 1;
		else if (flag) {
			buf->par_size++;
			flag = 0;
		}
	}
	buf->parsed = (char**) calloc(buf->par_size + 1, sizeof(char*));
	buf->programs = (char***) calloc(buf->program_n, sizeof(char**));
	for (int i = 0, word = 0; i < buf->len;) {
		while (buf->buffer[i] == ' ') i++;
		buf->parsed[word++] = buf->buffer + i;
		while (buf->buffer[i] != ' ' && buf->buffer[i] != '\n') i++;
		buf->buffer[i++] = '\0';
	}
	buf->parsed[buf->par_size] = NULL;
	buf->programs[0] = buf->parsed;
	for (int i = 1, prog = 1; i < buf->par_size; i++) {
		if (strcmp(buf->parsed[i], "|") == 0) {
			buf->programs[prog++] = buf->parsed + i + 1;
			buf->parsed[i] = NULL;
		}
	}
}

void execute_files(struct Buf* buf) {
	int* fd = (int*) calloc(buf->program_n * 2, sizeof(int));
	fd[0] = 0;
	fd[buf->program_n * 2 - 1] = 1;

	for (int i = 0; i < buf->program_n; i++) {
		if (i < buf->program_n - 1) {
			int x[2];
			pipe(x);
			fd[2 * i + 1] = x[1];
			fd[2 * i + 2] = x[0];
		}
		if (fork() == 0) {
			dup2(fd[2 * i], 0);
			dup2(fd[2 * i + 1], 1);
			if (execvp(buf->programs[i][0], buf->programs[i]) < 0) {
				perror("Can't execute program");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		if (i > 0) close(fd[2 * i]);
		if (i < buf->program_n - 1) close(fd[2 * i + 1]);
	}
	for (int i = 0; i < buf->program_n; i++) wait(NULL);
	free(fd);
}

int main() {
	struct Buf buf;
	buf.buf_size = BUFFER_SIZE;
	buf.buffer = (char*) malloc(buf.buf_size);
	buf.signal = 1;

	while (buf.signal) {
		write(1, "$ ", 2);
		buf.len = getline(&buf.buffer, &buf.buf_size, stdin);
		if (strncmp(buf.buffer, "exit", 4) == 0) {
			break;
		}
		parse(&buf);
		execute_files(&buf);
		free(buf.parsed);
		free(buf.programs);
	}
	free(buf.buffer);
}
