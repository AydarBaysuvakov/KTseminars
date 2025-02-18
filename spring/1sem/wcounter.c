#include <stdio.h>
#include <assert.h>
#include "wcounter.h"
#include "parser.h"
#include "hashtable.h"
#include "array.h"

void word_count(FILE * fd)
{
    assert(fd);
    struct parser * psr = parser_ctor(fd);
    struct hashtable * ht = hashtable_ctor();

    char word[WORD_LEN] = "";
    while (get_word(psr, word))
    {
        ht_add_word(ht, word);
    }
    
    struct array * arr = array_ctor(ht);
    sort(arr, ht);

    unsigned count = 0;
    for (int i = 0; i < arr->array_len; i++)
    {
        if (count = ht_get_count(ht, arr->arr[i]))
        {
            printf("%s: %d\n", ht_get_word(ht, arr->arr[i]), count);
        }
    }

    array_dtor(arr);
    parser_dtor(psr);
    hashtable_dtor(ht);
}

