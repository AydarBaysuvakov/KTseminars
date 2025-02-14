#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "hashtable.h"

struct hashtable 
{
    struct table_elem * table[HASHTABLE_SIZE];
    char * buffer[BUFFER_LENGHT];
    char * buffer_ptr;
    unsigned iter;
};

struct table_elem
{
    unsigned long hash;
    char *        word;
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
    memset(ht, 0, sizeof(struct hashtable));
    return ht;
}

void hashtable_dtor(struct hashtable * ht)
{
    assert(ht);

    for (int i = 0; i < HASHTABLE_SIZE; ++i) 
    {
        if (ht->table[i] != NULL) 
            free(ht->table[i]);
    }
    free(ht);
}

int ht_find_word(struct hashtable * ht, const char * word)
{
    assert(ht);

    unsigned hash = hash_function(word);
    if (ht->table[hash] != NULL) 
        return 1;
    return 0;
}

void ht_add_word(struct hashtable * ht, const char * word)
{
    assert(ht);

    unsigned hash = hash_function(word);
    if (ht->table[hash] != NULL) 
        {
            ht->table[hash]->counter++;
            return;
        }
    strncpy(ht->buffer_ptr, word, WORD_LEN);
    ht->table[hash] = (struct table_elem *) malloc(sizeof(struct table_elem));
    ht->table[hash]->counter = 1;
    ht->table[hash]->hash    = hash;
    ht->table[hash]->word    = ht->buffer_ptr;
    ht->buffer_ptr += WORD_LEN;
}

int ht_print_word(struct hashtable * ht)
{
    while (ht->iter < HASHTABLE_SIZE && ht->table[ht->iter] == NULL) ;
    if (ht->iter == HASHTABLE_SIZE) 
    {
        ht->iter = 0;
        return -1;
    }
    printf("%s\t%d\n", ht->table[ht->iter]->word, ht->table[ht->iter]->counter);
    return 0;
}