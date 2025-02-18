#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "parser.h"

struct parser
{
    FILE *       fd;
};

struct parser * parser_ctor(FILE * fd) 
{
    assert(fd);
    struct parser * p = (struct parser *) malloc(sizeof(struct parser));
    if (!p)
    {
        perror("Cannot allocate memory for parser");
        return NULL;
    }
    p->fd = fd;
    return p;
}

void parser_dtor(struct parser * p)
{
    assert(p);
    free(p);
}

char * get_word(struct parser * p, char * word) 
{
    assert(p);
    if(fscanf(p->fd, "%s", word) < 0) return NULL;

    for (int i = 0; i < strlen(word); ++i) 
        word[i] = tolower(word[i]);
    return word;
}