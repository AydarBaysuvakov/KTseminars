#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

const size_t BUFFER_SIZE = 1024;
const char shm_filename[]   = "pccat_shm";
const char semf_filename[]  = "pccat_semf";
const char seme_filename[]  = "pccat_seme";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect input");
        return 0;    
    }

    int file = open(argv[1], O_CREAT | O_WRONLY);
    if (file < 0) {
        perror("file open failed");
        return 0;
    }

    int fd = shm_open(shm_filename, O_CREAT | O_RDWR, S_IRUSR);
    if (fd < 0) {
        perror("shm_open failed");
        close(file);
        return 0;
    }

    if (ftruncate(fd, BUFFER_SIZE) < 0) {
        perror("ftruncate failed");
        close(file);
        return 0;
    }

    char *shmp = (char*) mmap(NULL, BUFFER_SIZE, PROT_READ, MAP_SHARED_VALIDATE, fd, 0);
    if (shmp == MAP_FAILED) {
        perror("mmap failed");
        close(file);
        return 0;
    }
 
    sem_t *semf_ptr = sem_open(semf_filename, O_CREAT, 0777, 0);
    if (semf_ptr == SEM_FAILED) {
        perror("sem_open failed");
        close(file);
        return 0;
    }

    sem_t *seme_ptr = sem_open(seme_filename, O_CREAT, 0777, 1);
    if (seme_ptr == SEM_FAILED) {
        perror("sem_open failed");
        close(file);
        return 0;
    }

	while (1) {
        sem_wait(semf_ptr);
	if (shmp[0] == 0) break;
        int wr = write(file, shmp, BUFFER_SIZE);
        if (wr < 0) {
            perror("Write failed");
            close(file);
            return 0;
        }
        else if (wr == 0) break;
        sem_post(seme_ptr);
	}
    close(file);
    
    if (munmap(shmp, BUFFER_SIZE) < 0) {
        perror("munmap failed");
    }
    shm_unlink(shm_filename);
    sem_unlink(seme_filename);
    sem_unlink(semf_filename);

}
