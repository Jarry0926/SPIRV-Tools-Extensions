#include "StrMap.h"

inline void
strmapInit(struct StrMap* const p_pStrMap)
{
	memset(p_pStrMap->b,
		   0,
		   1024 * sizeof(struct Bucket));
	p_pStrMap->pPoolIt = p_pStrMap->pool;
}

inline uint64_t
strmapHash(const char* p_key)
{
	// FNV-1 hash to high 32 bits
	constexpr uint32_t kFnv1Prime = 16777619u;
	constexpr uint32_t kFnv1Base  = 2166136261u;
	uint32_t fnv1Val = kFnv1Base;
	for (const char* it = p_key; *it != '\0'; ++it) {
		fnv1Val = fnv1Val * kFnv1Prime ^ *it;
	}
	// DJB2 hash to low 32 bits
	constexpr uint32_t kDjb2Prime = 33u;
	constexpr uint32_t kDjb2Base  = 5381u;
	uint32_t djb2Val = kDjb2Base;
	for (const char* it = p_key; *it != '\0'; ++it) {
		djb2Val = djb2Val * kDjb2Prime + *it;
	}
	return ((uint64_t)fnv1Val << 32) + djb2Val;
}

inline void
strmapPush(struct StrMap* const p_pStrMap,
		   const char*          p_key,
		   const uint64_t       p_val)
{
	auto b = &p_pStrMap->b[p_val & 1023ull];
	// Navigate to the tail of the chain
	while (b->next != nullptr) {
		b = b->next;
	}
	// Assign and extent it
	b->next = p_pStrMap->pPoolIt;
	memcpy(b->key, p_key, strlen(p_key) * sizeof(char));
	b->val = p_val;
	p_pStrMap->pPoolIt->next = nullptr;
	++p_pStrMap->pPoolIt;
}

inline int
strmapFind(struct StrMap* const p_pStrMap,
		   const char*          p_key,
	       const uint64_t       p_val)
{
	for (auto b = &p_pStrMap->b[p_val & 1023u];
		 b->next != nullptr;
		 b = b->next)
	{
		if (strcmp(b->key, p_key) == 0) {
			return 1;
		}
	}
	return 0;
}

