#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Convert a 64-bit hex integer into string
 *
 * @param p_in: hex integer to be converted
 * @param p_pOut: string to be convered into
 */
void
strFromHex64(uint64_t p_in,
		     char*    p_pOut);

/**
 * @brief Strip white spaces from `*p_ppIn` until `p_end` delimiter,
 *        producing `p_pOut`, advancing `*p_ppIn` to the last char scanned
 *
 * @param p_ppIn: pointer to string to be stripped
 * @param p_pOut: string after being stripped
 * @param p_end: ending delimiter of `p_pIn`
 *
 * @return pointer pass the end of `p_pOut`
 */
char*
strStrip(char const** const p_ppIn,
		 char*              p_pOut,
		 const char         p_end);

/**
 * @brief Strip white spaces from the first `p_len`th chars in `p_pIn`,
 *        producing `p_pOut`
 *
 * @param p_pIn: string to be stripped
 * @param p_pOut: string after being stripped
 * @param p_len: number of chars in `p_pIn`, excluding null terminator
 *
 * @return pointer pass the end of `p_pOut`
 */
char*
strStripN(char const* const p_pIn,
		  char*             p_pOut,
		  const uint32_t    p_len);


/**
 * @brief Check whether or not two strings `p_pSrc` and `p_pDst` are the same
 *
 * @param p_pSrc: source string to be compared, must be 64-byte aligned
 * @param p_pDst: destination string to be compared, must be 64-byte aligned
 *
 * @return 1 if they are the same,
 *         0 otherwise
 */
int
str512Match(char const* p_pSrc,
	        char const* p_pDst);

/**
 * @breif Check whether or not a pattern `p_pPat` occurs in string `p_pStr`
 *
 * @param p_pPat: patter to be checked, must be 64-byte aligned
 * @param p_pStr: string to be checked, must be 64-byte aligned
 *
 * @return index into `p_pStr` where the pattern occurs,
 *         0 if the pattern does not occur
 */
int
str512SubMatch(char const*    p_pPat,
		       char const*    p_pStr,
			   const uint32_t p_patLen);

#if defined(__cplusplus)
} // extern "C"
#endif

