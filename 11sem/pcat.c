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

    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        perror("file open failed");
        return 0;
    }

    int fd = shm_open(shm_filename, O_CREAT | O_RDWR, S_IWUSR);
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

    char *shmp = (char*) mmap(NULL, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
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
        sem_wait(seme_ptr);
        int rd = read(file, shmp, BUFFER_SIZE);
        if (rd < 0) {
            perror("Read failed");
            close(file);
            return 0;
        }
        else if (rd == 0) {
		shmp[0] = 0;
		sem_post(semf_ptr);
		break;
	}
        sem_post(semf_ptr);
	}
    close(file);
    
    if (munmap(shmp, BUFFER_SIZE) < 0) {
        perror("munmap failed");
    }
    shm_unlink(shm_filename);
    sem_unlink(seme_filename);
    sem_unlink(semf_filename);

}
