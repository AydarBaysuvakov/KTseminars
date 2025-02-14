#ifndef ANIMAL
#define ANIMAL

#define NAME_LEN 32

struct animal_m 
{
    void (*set_name)(struct animal *, char *);
    const char * (*get_name)(struct animal *);
    void (*voice)(struct animal *);
};

struct animal 
{
    struct animal_m methods;
    char name[NAME_LEN];
};

void set_name(struct animal *, char *);
const char * get_name(struct animal *);

#endif//ANIMAL