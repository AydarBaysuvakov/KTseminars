#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

const size_t BUFFER_SIZE = 1024;

int main() {
	int shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0777 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget error");
        return 0;
    }

    char* p = (char*) shmat(shmid, NULL, 0);
    if ((long) p < 0) {
        perror("shmat error");
        return 0;
    }

    if (fork() == 0) {
        strcpy(p, "Hello world\n");
        return 0;
    }

    wait(NULL);
    puts(p);

    if (shmdt(p) < 0) {
        perror("shmdt error");
    }

	if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmclt error");
    }
}
