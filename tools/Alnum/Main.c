#include <stdio.h>

#include <Alnum/Alum.h>
#include "ArgSet.h"

int
main(int p_numArgs, char** p_ppArgs)
{
	struct ArgSet as;
	argSetInit(&as, p_ppArgs);
	
	spvAlnum();
	argSetDestroy(&as);
	return 0;
}

