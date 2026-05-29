#pragma once

#include <stdint.h>
#include <string.h>

struct Buck
{
	struct Buck* next;
	char 		 key[256];
	uint32_t     val;
}; // struct Buck

struct StrMap
{
	struct Buck  Bucks[1024];
	struct Buck  BuckPool[1024];
	struct Buck* BuckPoolIt;
}; // struct StrMap

/**
 * @breif Initialize global variables
 */
void
strMapInit(struct StrMap* const p_strMap);

/**
 * @brief Hash p_key into a uint64_t value
 *
 * @param p_key: string (key) to be hashed
 * @return the hash value
 */
uint32_t
strMapHash(const char* p_key);

/**
 * @brief Push (p_key, p_val) into buckets
 *
 * @param p_key: key of the pair
 * @param p_val: value of the pair
 */
void
strMapPush(struct StrMap* const p_strMap,
		   const char*          p_key,
	       const uint32_t       p_val);

/**
 * @brief Find (p_key, p_val) in buckets
 *
 * @param p_key: key of the pair
 * @param p_val: value of the pair
 * @return 1 if found, 0 otherwise
 */
int
strMapFind(struct StrMap* const p_strMap,
		   const char*          p_key,
	       const uint32_t       p_val);

