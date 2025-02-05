#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

const size_t BUFFER_SIZE = 1024;
const size_t SEM_COUNT = 8;

int main() {
    int shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0777 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget error");
        return 0;
    }

	int semid = semget(IPC_PRIVATE, SEM_COUNT, 0777 | IPC_CREAT);
    if (semid < 0) {
        perror("semget error");
        return 0;
    }

    char* p = (char*) shmat(shmid, NULL, 0);
    if ((long) p < 0) {
        perror("shmat error");
        return 0;
    }

    if (semctl(semid, 0, SETVAL, 1) < 0) {
        perror("semclt error");
    }

    if (fork() == 0) {
        strcpy(p, "Hello world\n");
        struct sembuf sop1 = {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
        semop(semid, &sop1, 1);
        return 0;
    }
    struct sembuf sop2 = {.sem_num = 0, .sem_op = 0, .sem_flg = 0};
    semop(semid, &sop2, 1);
    puts(p);

    if (shmdt(p) < 0) {
        perror("shmdt error");
    }

	if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmclt error");
    }
}
