#include <stdio.h>

#include "src/Substitude.h"

int
main(int p_numArgs, char** p_args)
{
    printf("%s", parseRead(p_args[1]));
    return 0;
}
