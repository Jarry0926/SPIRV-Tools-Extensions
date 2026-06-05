#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

struct Bucket
{
	char 		   key[256];
	struct Bucket* next;
	uint64_t       val;
}; // struct Buck

struct StrMap
{
	struct Bucket  b[1024];
	struct Bucket  pool[1024];
	struct Bucket* pPoolIt;
}; // struct StrMap

/**
 * @breif Initialize global variables
 * 
 * @param p_pStrMap: object to initialize
 */
void
strmapInit(struct StrMap* const p_pStrMap);

/**
 * @brief Hash p_key into a uint64_t value
 *
 * @param p_key: string (key) to be hashed
 *
 * @return the hash value
 */
uint64_t
strmapHash(const char* p_keyey);

/**
 * @brief Push (p_key, p_val) into hash buckets
 *
 * @param p_pStrMap: object it works on
 * @param p_key: key of the pair
 * @param p_val: value of the pair
 */
void
strmapPush(struct StrMap* const p_pStrMap,
		   const char*          p_key,
	       const uint64_t       p_val);

/**
 * @brief Find (p_key, p_val) in hash buckets
 *
 * @param p_pStrMap: object it works on
 * @param p_key: key of the pair
 * @param p_val: value of the pair
 *
 * @return 1 if found, 0 otherwise
 */
int
strmapFind(struct StrMap* const p_pStrMap,
		   const char*          p_key,
	       const uint64_t       p_val);

#if defined(__cplusplus)
} // extern "C"
#endif

