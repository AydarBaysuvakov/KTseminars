#include "animal.h"
#include "dog.h"
#include <stdio.h>
#include <stdlib.h>

struct dog
{
    struct animal a;
};

struct dog * dog_create(char * name)
{
    struct dog * d = (struct dog *) malloc(sizeof(struct dog));
    struct animal * a = (struct animal *) d;
    a->methods.set_name =  set_name;
    a->methods.get_name =  get_name;
    a->methods.voice    = (void (*)(struct animal *)) dog_voice;
    a->methods.set_name(a, name);
    return d;
}

void dog_destroy(struct dog * d) 
{
    free(d);
}

void dog_voice(struct dog * d)
{
    struct animal * a = (struct animal *) d;
    const char * name = a->methods.get_name(a);
    printf("Dog %s bark!\n", name);
}
