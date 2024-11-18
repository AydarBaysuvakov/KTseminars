#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

const int PIZZA_COUNT = 10000;

struct Pizza {
    char storage[PIZZA_COUNT][5];
    int curent_pizza;
    int top_ingredient;
    int courier_pizza;
    pthread_mutex_t m;
    pthread_cond_t  c;
};

void * put_d(void *arg) {
    struct Pizza * a = (struct Pizza*) arg;
    while ((a->storage)[curent_pizza][top_ingredient] == "o") {
        pthread_mutex_lock(&a->m);
        strncat((a->storage)[curent_pizza], "d", 1);
        pthread_mutex_unlock(&a->m);
    }
    return a;
}

void * get_pizza(void *arg) {
	struct Counter * a = (struct Counter*) arg;
	pthread_mutex_lock(&a->m);
	sleep(1);
	printf("Счетчик: %d\n", a->count);
	pthread_mutex_unlock(&a->m);
	return a;
}

void povar_d(struct Pizza * pizza) {
    pthread_t thread;
    for (int i = 0; i < PIZZA_COUNT; i++) {
	create_pthread(&thread, NULL, put_d, pizza);
	join_pthread(pthread, NULL);
    }
}

void povar_o(struct Pizza * pizza) {}

void courier(struct Pizza * pizza) {}

void consumer(struct Pizza * pizza) {
    pthread_t thread;
    for (int i = 0; i < PIZZA_COUNT, i++) {
    	create_pthread(&thread, NULL, get_pizza, pizza);
	join_pthread(pthread, NULL);
    }
}

int main() {
    struct Pizza pizza;
    pthread_mutex_init(&pizza.m, NULL);
    pthread_cond_init(&pizza.c, NULL);

    povar_d(&pizza);
    povar_o(&pizza);
    courier(&pizza);
    consumer(&pizza);
}
