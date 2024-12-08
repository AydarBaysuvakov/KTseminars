#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

const int THREAD_COUNT = 20000;

struct Counter {
    int count;
    pthread_mutex_t m;
};

void * counter(void *arg) {
    struct Counter * a = (struct Counter*) arg;
    pthread_mutex_lock(&a->m);
    (a->count)++;
    pthread_mutex_unlock(&a->m);
    return a;
}

void * write(void *arg) {
	struct Counter * a = (struct Counter*) arg;
	pthread_mutex_lock(&a->m);
	sleep(1);
	printf("Счетчик: %d\n", a->count);
	pthread_mutex_unlock(&a->m);
	return a;
}

int main() {
    pthread_t thread[THREAD_COUNT];
    pthread_t write_thread;

    struct Counter arg;
    arg.count = 0;
    pthread_mutex_init(&arg.m, NULL);
    
    for (int i = 0; i < THREAD_COUNT; i++) {
    	pthread_create(thread + i, NULL, counter, &arg);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(thread[i], NULL);
    }

    pthread_create(&write_thread, NULL, write, &arg);
    pthread_join(write_thread, NULL);

    pthread_mutex_destroy(&arg.m);
}
