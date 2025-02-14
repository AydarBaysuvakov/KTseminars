#ifndef DOG
#define DOG

struct dog;

struct dog * dog_create(char *);
void dog_destroy(struct dog *);
void dog_voice(struct dog *);

#endif//DOG