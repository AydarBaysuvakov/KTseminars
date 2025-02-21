#ifndef ARRAY
#define ARRAY

#include "hashtable.h"

typedef struct array
{
    unsigned long * arr;
    unsigned array_len;
} array;

array * array_ctor(hashtable *);
void    array_dtor(array *);
void    sort(array *, hashtable *);

#endif //ARRAY