#ifndef PARSER
#define PARSER

#define WORD_LEN 128

typedef struct parser parser;

parser * parser_ctor(const char *);
void     parser_dtor(parser *);
char *   get_word(parser *, char *);

#endif //PARSER