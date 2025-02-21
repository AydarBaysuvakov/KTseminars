#include <stdio.h>
#include "wcounter.h"

int main(int argc, char* argv[])
{
    if (argc < 2) 
        word_count(NULL);
    else
        word_count(argv[1]);
}