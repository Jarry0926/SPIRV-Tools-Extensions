#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "ArgSet.h"

/**
 * @brief Parse the file path and open input file
 *
 * @param p_pArgSet: object worked on
 * @param p_pArg: argument to be parsed
 *
 * @return >0 on success, 0 on skip, <0 on failure
 */
static inline int
initParseFpIn(struct ArgSet* const p_pArgSet,
			  char const*          p_pArg)
{
	const uint32_t n = strlen(p_pArg);
	if (strncmp("-", p_pArg, 2) == 0) {
		p_pArgSet->fpIn = stdin;
		return 1;
	}
	else if (strncmp(".spvasm", &p_pArg[n - 7], 8) == 0) {
		p_pArgSet->fpIn = fopen(p_pArg, "r");
		if (p_pArgSet->fpIn == nullptr) {
			fprintf(stderr,
					"Failed to open %s as input: %s\n",
					p_pArg, strerror(errno));
			return -1;
		}
		return 1;
	}
	return 0;
}

/**
 * @brief Parse "-o" and open output file through the next argument
 *
 * @param p_pArgSet: object worked on
 * @param p_ppArgs: pointer to the argument to be parsed
 *
 * @return >0 on success, 0 on skip, <0 on failure
 */
static inline int
initParseFpOut(struct ArgSet* const p_pArgSet,
		       char**               p_ppArgs)
{
	if (strncmp("-o", p_ppArgs[0], 3) == 0) {
		if (p_ppArgs[1] == nullptr) {
			fputs("Missing output path\n", stderr);
			return -1;
		}
		else if (strncmp("-", p_ppArgs[1], 2) == 0) {
			p_pArgSet->fpOut = stdout;
			return 1;
		}
		p_pArgSet->fpOut = fopen(p_ppArgs[1], "w");
		if (p_pArgSet->fpOut == nullptr) {
			fprintf(stderr,
					"Failed to open %s at output: %s\n",
	  				p_ppArgs[1], strerror(errno));
			return -1;
		}
		return 1;
	}
	return 0;
}

/**
 * @brief Parse "-g"
 *
 * @param p_pArgSet: object worked on
 * @param p_pArg: argument to be parsed
 *
 * @return >0 on success, 0 on skip
 */
static inline int
initParseDebugInfo(struct ArgSet* const p_pArgSet,
				   char const*          p_pArg)
{
	if (strncmp("-g", p_pArg, 3) == 0) {
		p_pArgSet->optMask |= ALNUM_OPT_DBG_INFO_BIT;
		return 1;
	}
	return 0;
}


inline int
argSetInit(struct ArgSet* const p_pArgSet,
		   char**               p_ppArgs)
{
	memset(p_pArgSet, 0, sizeof(struct ArgSet));
	char** ppIt = p_ppArgs;
	while (ppIt != nullptr) {
		int result;
		if ((result = initParseFpIn(p_pArgSet, ppIt[0]))) {
			if (result < 0) {
				goto OnError;
			}
			++ppIt;
		}
		else if ((result = initParseFpOut(p_pArgSet, ppIt))) {
			if (result < 0) {
				goto OnError;
			}
			ppIt += 2;
		}
		else if ((result = initParseDebugInfo(p_pArgSet, ppIt[0]))) {
			if (result < 0) {
				goto OnError;
			}
			++ppIt;
		}
		else {
			fprintf(stderr, "Unknown argument %s\n", ppIt[0]);
			goto OnError;
		}
	}
	return 0;
OnError:
	argSetDestroy(p_pArgSet);
	return -1;
}

inline void
argSetDestroy(struct ArgSet* const p_pArgSet)
{
	if (p_pArgSet->fpIn != nullptr) {
		fclose(p_pArgSet->fpIn);
	}
	if (p_pArgSet->fpOut != nullptr) {
		fclose(p_pArgSet->fpOut);
	}
}

