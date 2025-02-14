#include "animal.h"
#include "cat.h"
#include <stdio.h>
#include <stdlib.h>

struct cat 
{
    struct animal a;
};

struct cat * cat_create(char * name)
{
    struct cat * c = (struct cat *) malloc(sizeof(struct cat));
    struct animal * a = (struct animal *) c;
    a->methods.set_name =  set_name;
    a->methods.get_name =  get_name;
    a->methods.voice    = (void (*)(struct animal *)) cat_voice;
    a->methods.set_name(a, name);
    return c;
}

void cat_destroy(struct cat * c) 
{
    free(c);
}

void cat_voice(struct cat * c)
{
    struct animal * a = (struct animal *) c;
    const char * name = a->methods.get_name(a);
    printf("Cat %s meow!\n", name);
}