#include <string.h>
#include "animal.h"

void set_name(struct animal * apt, char * name)
{
    strncpy(apt->name, name, NAME_LEN);
}

const char * get_name(struct animal * apt)
{
    return apt->name;
}