#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

const int STORAGE_SIZE = 100;
const int PIZZA_COUNT  = 10000;

struct Pizza {
    char storage[STORAGE_SIZE][6];
    char courier_bag[6];
    int top;
    int bottom;
    int cur_let;
    int correct;
    int overal;
    int baked;
    int check;
    pthread_mutex_t m;
};

void * put_d(void *arg) {
    struct Pizza * pizza = (struct Pizza*) arg;
    for (int i = 0; i < PIZZA_COUNT; i++) {
        while ((pizza->storage)[pizza->top][0] != 0) ;

        pthread_mutex_lock(&pizza->m);
        strncat((pizza->storage)[pizza->top], "d", 2);
        pizza->cur_let++;
        pthread_mutex_unlock(&pizza->m);

        while (pizza->cur_let != 2) ;

        pthread_mutex_lock(&pizza->m);
        strncat((pizza->storage)[pizza->top], "d", 2);
        pizza->cur_let++;
        pthread_mutex_unlock(&pizza->m);

        while (pizza->cur_let != 4) ;

        pthread_mutex_lock(&pizza->m);
        strncat((pizza->storage)[pizza->top], "\n", 2);
        pizza->top++;
        pizza->top %= STORAGE_SIZE;
        pizza->cur_let = 0;
        pizza->baked++;
        printf("Baked pizza №%d\n", pizza->baked);
        pthread_mutex_unlock(&pizza->m);
    }
    return pizza;
}

void * put_o(void *arg) {
    struct Pizza * pizza = (struct Pizza*) arg;
    for (int i = 0; i < PIZZA_COUNT; i++) {
        while (pizza->cur_let != 1) ;

        pthread_mutex_lock(&pizza->m);
        strncat((pizza->storage)[pizza->top], "o", 2);
        pizza->cur_let++;
        pthread_mutex_unlock(&pizza->m);

        while (pizza->cur_let != 3) ;

        pthread_mutex_lock(&pizza->m);
        strncat((pizza->storage)[pizza->top], "o", 2);
        pizza->cur_let++;
        pthread_mutex_unlock(&pizza->m);
    }
    return pizza;
}

void * send_pizza(void *arg) {
	struct Pizza * pizza = (struct Pizza*) arg;
	for (int i = 0; i < PIZZA_COUNT; i++) {
        while (pizza->check == 1) ;
        while ((pizza->storage)[pizza->bottom][4] == 0) ;

        pthread_mutex_lock(&pizza->m);
        strncpy(pizza->courier_bag, (pizza->storage)[pizza->bottom], 5);
        memset((pizza->storage)[pizza->bottom], 0, 6);
        pizza->bottom++;
        pizza->bottom %= STORAGE_SIZE;
        pizza->check = 1;
        pthread_mutex_unlock(&pizza->m);
    }
	return pizza;
}

void * get_pizza(void *arg) {
	struct Pizza * pizza = (struct Pizza*) arg;
	for (int i = 0; i < PIZZA_COUNT; i++) {
        while (pizza->check == 0) ;
        pthread_mutex_lock(&pizza->m);
        pizza->overal++;
        printf("Pizza №%d is ", pizza->overal);
        if (strcmp(pizza->courier_bag, "dodo\n")) {
            printf("incorrect, get %s\n", pizza->courier_bag);
        }
        else {
            printf("correct, get %s", pizza->courier_bag);
            pizza->correct++;
        }
        memset(pizza->courier_bag, 0, 6);
        pizza->check = 0;
        pthread_mutex_unlock(&pizza->m);
    }
    printf("Correct amount of pizza: %d\n", pizza->correct);
	return pizza;
}

void povar_d(struct Pizza * pizza) {
    pthread_t thread;
	pthread_create(&thread, NULL, put_d, pizza);
}

void povar_o(struct Pizza * pizza) {
    pthread_t thread;
	pthread_create(&thread, NULL, put_o, pizza);
}

void courier(struct Pizza * pizza) {
    pthread_t thread;
	pthread_create(&thread, NULL, send_pizza, pizza);
}

void consumer(struct Pizza * pizza) {
    pthread_t thread;
	pthread_create(&thread, NULL, get_pizza, pizza);
	pthread_join(thread, NULL);
}

int main() {
    struct Pizza pizza = {.top = 0, .bottom = 0, .cur_let = 0, .correct = 0, .overal = 0, .baked = 0, .check = 0};
    memset(pizza.storage, 0, STORAGE_SIZE * 6);
    memset(pizza.courier_bag, 0, 6);
    pthread_mutex_init(&pizza.m, NULL);

    povar_d(&pizza);
    povar_o(&pizza);
    courier(&pizza);
    consumer(&pizza);

    pthread_mutex_destroy(&pizza.m);
}
