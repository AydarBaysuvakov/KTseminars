#ifndef HASH
#define HASH

#define HASHTABLE_SIZE 0x100000

struct hashtable;
struct table_elem;

unsigned hash_function(const char *);
struct hashtable * hashtable_ctor();
void hashtable_dtor(struct hashtable *);
int ht_find_word(struct hashtable *, const char *);
int ht_print_word(struct hashtable *);
void ht_add_word(struct hashtable *, const char *);

#endif //HASH