#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

const size_t BUFFER_SIZE = 1024;

struct Shell_t {
	size_t buffer_size;
	
	size_t len;
	char* buffer;
	
	char** parsed_line;
	size_t words_count;
	
	char*** programs;
	size_t program_count;
	
	int signal;
};

void parse_line(struct Shell_t* shell) {
	int flag = 1; 
	shell->words_count = 0;
	shell->program_count = 1;
	for (int i = 0; i < shell->len; i++) {
		if (shell->buffer[i] == '|') shell->program_count++;
		if (shell->buffer[i] == ' ') flag = 1;
		else if (flag) {
			shell->words_count++;
			flag = 0;
		}
	}
	shell->parsed_line = (char**) calloc(shell->words_count + 1, sizeof(char*));
	shell->programs = (char***) calloc(shell->program_count, sizeof(char**));
	for (int i = 0, word = 0; i < shell->len;) {
		while (shell->buffer[i] == ' ') i++;
		shell->parsed_line[word++] = shell->buffer + i;
		while (shell->buffer[i] != ' ' && shell->buffer[i] != '\n') i++;
		shell->buffer[i++] = '\0';
	}
	shell->parsed_line[shell->words_count] = NULL;
	shell->programs[0] = shell->parsed_line;
	for (int i = 1, prog = 1; i < shell->words_count; i++) {
		if (strcmp(shell->parsed_line[i], "|") == 0) {
			shell->programs[prog++] = shell->parsed_line + i + 1;
			shell->parsed_line[i] = NULL;
		}
	}
}

void execute_files(struct Shell_t* shell) {
	int* fd = (int*) calloc(shell->program_count * 2, sizeof(int));
	fd[0] = 0;
	fd[shell->program_count * 2 - 1] = 1;

	for (int i = 0; i < shell->program_count; i++) {
		if (i < shell->program_count - 1) {
			int x[2];
			if (pipe(x) < 0) {
				perror("pipe error");
				exit(EXIT_FAILURE);
			}
			fd[2 * i + 1] = x[1];
			fd[2 * i + 2] = x[0];
		}
		if (fork() == 0) {
			dup2(fd[2 * i], 0);
			dup2(fd[2 * i + 1], 1);
			if (execvp(shell->programs[i][0], shell->programs[i]) < 0) {
				perror("Can't execute program");
				shell->signal = 0;
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else {
			int wstatus;
			wait(&wstatus);
			if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == EXIT_FAILURE) shell->signal = 0;
		}
		if (i > 0) close(fd[2 * i]);
		if (i < shell->program_count - 1) close(fd[2 * i + 1]);
	}
	for (int i = 0; i < shell->program_count; i++) wait(NULL);
	free(fd);
}

int main() {
	struct Shell_t shell;

	shell.buffer_size = BUFFER_SIZE;
	shell.buffer = (char*) malloc(shell.buffer_size);
	
	shell.signal = 1;

	while (shell.signal) {
		write(1, "$ ", 2);

		shell.len = getline(&shell.buffer, &shell.buffer_size, stdin);
		if (shell.len < 0) {
			perror("getline error");
			break;
		}
		if (strncmp(shell.buffer, "exit", 4) == 0) {
			break;
		}

		parse_line(&shell);
		execute_files(&shell);
		free(shell.parsed_line);
		free(shell.programs);
	}
	free(shell.buffer);
}
