#ifndef PARSER
#define PARSER

struct parser;

struct parser * parser_ctor(FILE *);
void parser_dtor(struct parser *);
char * get_word(struct parser *) ;

#endif //PARSER