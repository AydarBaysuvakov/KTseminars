#ifndef PARSER
#define PARSER

#define WORD_LEN 128

struct parser;

struct parser * parser_ctor(FILE *);
void parser_dtor(struct parser *);
char * get_word(struct parser *, char *) ;

#endif //PARSER