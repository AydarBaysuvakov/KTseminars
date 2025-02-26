#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "hashtable.h"

struct hashtable 
{
    table_elem * table[HASHTABLE_SIZE];
    char         buffer[BUFFER_LENGHT];
    unsigned     elem_count;
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

hashtable * hashtable_ctor() 
{
    hashtable * ht = (hashtable *) malloc(sizeof(hashtable));
    if (!ht)
    {
        perror("Cannot allocate memory for hash table");
        return NULL;
    }
    memset(ht, 0, sizeof(hashtable));
    return ht;
}

void hashtable_dtor(hashtable * ht)
{
    assert(ht);

    for (int i = 0; i < HASHTABLE_SIZE; ++i) 
    {
        if (ht->table[i] != NULL) 
            free(ht->table[i]);
    }
    free(ht);
}

void ht_add_word(hashtable * ht, const char * word)
{
    assert(ht);

    unsigned hash = hash_function(word);
    if (ht->table[hash] != NULL) 
        {
            ht->table[hash]->counter++;
            return;
        }
    strncpy(ht->buffer + hash * WORD_LEN, word, WORD_LEN);
    ht->table[hash] = (table_elem *) malloc(sizeof(table_elem));
    ht->table[hash]->counter = 1;
    ht->table[hash]->hash    = hash;
    ht->table[hash]->word    = ht->buffer + hash * WORD_LEN;
    ht->elem_count++;
}

char * ht_get_word(hashtable * ht, unsigned hash)
{
    assert(ht);

    if (ht->table[hash] != NULL) 
        return ht->table[hash]->word;
    return NULL;
}

unsigned ht_get_count(hashtable * ht, unsigned hash)
{
    assert(ht);

    if (ht->table[hash] != NULL) 
        return ht->table[hash]->counter;
    return 0;
}

unsigned ht_get_elem_count(hashtable * ht)
{
    assert(ht);
    return ht->elem_count;
}