#ifndef HASH
#define HASH

#define HASHTABLE_SIZE 0x10000
#define WORD_LEN 128
#define BUFFER_LENGHT WORD_LEN * HASHTABLE_SIZE

struct hashtable;
struct table_elem;

unsigned hash_function(const char *);
struct   hashtable * hashtable_ctor();
void     hashtable_dtor(struct hashtable *);
int      ht_find_word(struct hashtable *, const char *);
void     ht_add_word(struct hashtable *, const char *);
int      ht_print_word(struct hashtable *);

#endif //HASH