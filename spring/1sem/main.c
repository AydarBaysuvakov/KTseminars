#include <stdio.h>
#include "wcounter.h"

int main(int argc, char* argv[])
{
    
    if (argc < 2) 
    {
        word_count(stdin);
        return 0;
    }

    FILE * fd = fopen(argv[1], "r");
    word_count(fd);
    fclose(fd);
}