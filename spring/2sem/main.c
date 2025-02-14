#include "cat.h"
#include "dog.h"
#include "animal.h"
#include <stdio.h>

int main()
{
    struct cat * c = cat_create("Murka");
    struct dog * d = dog_create("Sharik");
    char letter = getchar();
    struct animal * ptr = (letter == 'c') ? (struct animal *) c : ((letter == 'd') ? (struct animal *) d : NULL);
    if (ptr)
    {
        ptr->methods.voice(ptr);
    }
    cat_destroy(c);
    dog_destroy(d);
}