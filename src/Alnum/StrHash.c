#include "StrHash.h"

#include <stdio.h>

inline void
strMapInit(struct StrMap* const p_strMap)
{
	memset(p_strMap->Bucks, 0, sizeof(struct Buck) << 10);
	p_strMap->BuckPoolIt = p_strMap->BuckPool;
}

inline uint32_t
strMapHash(const char* p_key)
{
	uint32_t val = 0u;
	for ( ; *p_key != '\0'; ++p_key) {
		val = (val * 33) + *p_key;
	}
	return val;
}

inline void
strMapPush(struct StrMap* const p_strMap,
		   const char*          p_key,
		   const uint32_t       p_val)
{
	auto b = &p_strMap->Bucks[p_val & 1023u];
	// Navigate to the tail of the chain
	while (b->next != nullptr) {
		b = b->next;
	}
	// Assign and extent it
	b->next = p_strMap->BuckPoolIt;
	memcpy(b->key, p_key, strlen(p_key) * sizeof(char));
	b->val  = p_val;
	(p_strMap->BuckPoolIt++)->next = nullptr;
}

inline int
strMapFind(struct StrMap* const p_strMap,
		   const char*          p_key,
	       const uint32_t       p_val)
{
	for (auto b = &p_strMap->Bucks[p_val & 1023u];
		 b->next != nullptr;
		 b = b->next)
	{
		if (strcmp(b->key, p_key) == 0) {
			return 1;
		}
	}
	return 0;
}

