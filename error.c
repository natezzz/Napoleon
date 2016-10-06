#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void
die(char const *why)
{
    perror(why);
    exit(-1);
}