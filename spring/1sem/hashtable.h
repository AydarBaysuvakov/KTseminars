#ifndef HASH
#define HASH

#define HASHTABLE_SIZE 0x10000
#define WORD_LEN 128
#define BUFFER_LENGHT WORD_LEN * HASHTABLE_SIZE

struct   hashtable;
struct   table_elem;
struct   array;

unsigned hash_function(const char *);
struct   hashtable * hashtable_ctor();
void     hashtable_dtor(struct hashtable *);

void     ht_add_word(struct hashtable *, const char *);
char *   ht_get_word(struct hashtable *, unsigned);
unsigned ht_get_count(struct hashtable *, unsigned);
unsigned ht_get_elem_count(struct hashtable *);

#endif //HASH