#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "hashtable.h"

void word_count(FILE * fd)
{
    assert(fd);
    struct parser * psr = parser_ctor(fd);
    struct hashtable * ht = hashtable_ctor();

    char * word = get_word(psr);
    while (word)
    {
        printf("%s\n", word);
        ht_add_word(ht, word);
        word = get_word(psr);
    }

    while (1)
    {
        //
    }

    parser_dtor(psr);
    hashtable_dtor(ht);
}

