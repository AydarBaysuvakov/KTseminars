#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "array.h"

array * array_ctor(hashtable * ht) 
{
    array * a = (array *) malloc(sizeof(array));
    if (!a)
    {
        perror("Cannot allocate memory for array");
        return NULL;
    }

    a->array_len = ht_get_elem_count(ht);
    a->arr = (unsigned long *) calloc(a->array_len, sizeof(unsigned long));
    int j = 0;
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (ht_get_count(ht, i))
        {
            a->arr[j++] = i;
        }
    }

    return a;
}

void array_dtor(array * a)
{
    assert(a);
    free(a);
}

// реализовал сортировку пузырьком для экономии своего личного времени (при необходимости могу написать сортировку получше)
// не применяю встроенные функции типа qsort, так как их тяжело примкнуть к хеш-таблице (если не менять саму хеш таблицу, что на мой взгляд ужасно)
// если не делать хеш-таблицу глобальной, то ее не получится вызвать в функции компараторе
void sort(array * a, hashtable * ht)
{
    assert(a);

    for (int i = 0; i < a->array_len - 1; i++) 
    {
        for (int j = 0; j < a->array_len - i - 1; j++)
        {
            if (ht_get_count(ht, a->arr[j]) < ht_get_count(ht, a->arr[j + 1])) 
            {
                unsigned long dummy = a->arr[j];
                a->arr[j] = a->arr[j + 1];
                a->arr[j + 1] = dummy;
            }
        }
    }
}