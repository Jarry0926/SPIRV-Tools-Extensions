#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>

#include <Alnum/Alnum.h>

struct ArgSet
{
	FILE*    fpIn;
	FILE*    fpOut;
	uint32_t optMask;
}; // struct ArgSet

/**
 * @brief Initialize a `struct ArgSet` object
 *
 * @param p_pArgSet: object to be initialized
 * @param p_ppArgs: arguments as string
 *
 * @return 0 on success, <0 on failure
 */
int
argSetInit(struct ArgSet* const p_pArgSet,
		   char** const         p_ppArgs);

/**
 * @brief Destroy a `struct ArgSet` object
 *
 * @param p_pArgSet: object to be destroied
 */
void
argSetDestroy(struct ArgSet* const p_pArgSet);

#if defined(__cplusplus)
} // extern "C"
#endif

