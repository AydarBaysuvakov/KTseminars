#ifndef HASH
#define HASH

#define HASHTABLE_SIZE 0x10000
#define WORD_LEN 128
#define BUFFER_LENGHT WORD_LEN * HASHTABLE_SIZE

typedef struct hashtable hashtable;
typedef struct table_elem table_elem;

unsigned    hash_function(const char *);
hashtable * hashtable_ctor();
void        hashtable_dtor(hashtable *);

void     ht_add_word(hashtable *, const char *);
char *   ht_get_word(hashtable *, unsigned);
unsigned ht_get_count(hashtable *, unsigned);
unsigned ht_get_elem_count(hashtable *);

#endif //HASH