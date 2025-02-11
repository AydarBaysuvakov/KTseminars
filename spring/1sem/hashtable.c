#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"
#include <stdio.h>

struct hashtable 
{
    struct table_elem * array[HASHTABLE_SIZE];
    int    iter;
};

struct table_elem
{
    unsigned long hash;
    const char *  word;
    unsigned      counter;
};

unsigned hash_function(const char *str)
{
    unsigned hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASHTABLE_SIZE;
}

struct hashtable * hashtable_ctor() 
{
    struct hashtable * ht = (struct hashtable *) malloc(sizeof(struct hashtable));
    memset(ht, 0, HASHTABLE_SIZE);
    ht->iter = 0;
    return ht;
}

void hashtable_dtor(struct hashtable * ht)
{
    assert(ht);

    for (int i = 0; i < HASHTABLE_SIZE; ++i) 
    {
        if (ht->array[i] != NULL) 
            free(ht->array[i]);
    }
    free(ht);
}

int ht_find_word(struct hashtable * ht, const char * word)
{
    assert(ht);

    unsigned hash = hash_function(word);
    if (ht->array[hash] != NULL) 
        return 1;
    return 0;
}

void ht_add_word(struct hashtable * ht, const char * word)
{
    assert(ht);

    unsigned hash = hash_function(word);
    printf("%d\n", hash);
    if (ht->array[hash] != NULL) 
        {
            ht->array[hash]->counter++;
            return;
        }
    ht->array[hash] = (struct table_elem *) malloc(sizeof(struct table_elem));
    ht->array[hash]->counter = 1;
    ht->array[hash]->hash    = hash;
    ht->array[hash]->word    = word;
    printf("add\n");
}

int ht_print_word(struct hashtable * ht)
{
    while (ht->iter < HASHTABLE_SIZE && ht->array[ht->iter] == NULL) ;
    if (ht->iter == HASHTABLE_SIZE) return -1;
    printf("%s\t%d\n", ht->array[ht->iter]->word, ht->array[ht->iter]->counter);
    return 0;
}