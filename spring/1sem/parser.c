#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "parser.h"

struct parser
{
    FILE * fd;
};

parser * parser_ctor(const char * file) 
{
    FILE * fd = stdin;
    if (file) 
    {
        fd = fopen(file, "r");
        if (!fd)
        {
            perror("Cannot open file");
            return NULL;
        }
    }

    parser * p = (parser *) malloc(sizeof(parser));
    if (!p)
    {
        perror("Cannot allocate memory for parser");
        fclose(fd);
        return NULL;
    }
    p->fd = fd;
    return p;
}

void parser_dtor(parser * p)
{
    assert(p);
    fclose(p->fd);
    free(p);
}

char * get_word(parser * p, char * word) 
{
    assert(p);
    if(fscanf(p->fd, "%s", word) < 0) return NULL;

    for (int i = 0; i < strlen(word); ++i) 
        word[i] = tolower(word[i]);
    return word;
}