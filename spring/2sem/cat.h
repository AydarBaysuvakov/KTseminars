#ifndef CAT
#define CAT

struct cat;

struct cat * cat_create(char *);
void cat_destroy(struct cat *);
void cat_voice(struct cat *);

#endif//CAT