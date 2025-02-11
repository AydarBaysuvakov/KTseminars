#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "parser.h"

struct parser
{
    FILE *       fd;
    unsigned     word_count;
};

struct parser * parser_ctor(FILE * fd) 
{
    assert(fd);
    struct parser * p = (struct parser *) malloc(sizeof(struct parser));
    p->fd = fd;
    p->word_count = 0;
    return p;
}

void parser_dtor(struct parser * p)
{
    assert(p);
    free(p);
}

char * get_word(struct parser * p) 
{
    assert(p);
    char * word;
    printf("hi\n");
    int len = fscanf(p->fd, "%s", word);
    perror("something went wrong");
    printf("hi\n");
    if (!len) return NULL;

    for (int i = 0; i < len; ++i)
    {
        word[i] = tolower(word[i]);
    }

    p->word_count++;
    printf("%s\n", word);
    return word;
}