#ifndef ARRAY
#define ARRAY

#include "hashtable.h"

struct array
{
    unsigned long * arr;
    unsigned array_len;
};

struct array * array_ctor(struct hashtable *);
void array_dtor(struct array *);
void sort(struct array *, struct hashtable *);

#endif //ARRAY