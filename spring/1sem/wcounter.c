#include <stdio.h>
#include <assert.h>
#include "wcounter.h"
#include "parser.h"
#include "hashtable.h"

void word_count(FILE * fd)
{
    assert(fd);
    struct parser * psr = parser_ctor(fd);
    struct hashtable * ht = hashtable_ctor();

    char word[WORD_LEN] = "";
    get_word(psr, word);
    while (*word)
    {
        ht_add_word(ht, word);
        get_word(psr, word);
    }

    while (ht_print_word(ht) != -1) ;

    parser_dtor(psr);
    hashtable_dtor(ht);
}

