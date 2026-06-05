#include <ctype.h>
#include <string.h>
#include <stdio.h>

#if defined(__AVX2__)
#include <immintrin.h>
#endif

#include "Str.h"

inline void
strFromHex64(uint64_t p_in,
		     char*    p_pOut)
{
	for (char* pIt = &p_pOut[15]; pIt >= p_pOut; --pIt) {
		const uint64_t digit = p_in & 15ull;
		*pIt = (digit < 10 ? '0' : 'A' - 10) + digit;
		p_in >>= 4;
	}
}

inline char*
strStrip(char const** const p_pIn,
		 char*              p_pOut,
		 const char         p_end)
{
	char const* pIt = *p_pIn;
	for (; *pIt != p_end; ++pIt) {
		if (!isspace(*pIt)) {
			*p_pOut = *pIt;
			++p_pOut;
		}
	}
	return p_pOut;
}

inline char*
strStripN(char const* const p_pIn,
		  char*             p_pOut,
		  const uint32_t    p_len)
{
	for (uint32_t i = 0u; i < p_len; ++i) {
		if (!isspace(p_pIn[i])) {
			*p_pOut = p_pIn[i];
			++p_pOut;
		}
	}
	return p_pOut;
}

#if defined(__AVX512F__)

static inline int
str512Match_avx512(char const* p_pSrc,
		           char const* p_pDst)
{
	p_pSrc = __builtin_assume_aligned(p_pSrc, 64);
	p_pDst = __builtin_assume_aligned(p_pDst, 64);

#if defined(__AVX512BW__)
	return UINT64_MAX ==
	_mm512_cmpeq_epu8_mask(_mm512_loadu_epi8(p_pSrc),
						   _mm512_loadu_epi8(p_pDst));
#else
	return UINT8_MAX ==
	_mm512_cmpeq_epu64_mask(_mm512_loadu_epi64(p_pSrc),
		                    _mm512_loadu_epi64(p_pDst));
#endif
}

#endif // __AVX512F__

#if defined(__AVX2__)

static inline int
str512Match_avx2(char const* p_pSrc,
		         char const* p_pDst)
{
	p_pSrc = __builtin_assume_aligned(p_pSrc, 32);
	p_pDst = __builtin_assume_aligned(p_pDst, 32);

	__m256i xor = _mm256_xor_si256(_mm256_load_si256((__m256i const*)p_pSrc),
							       _mm256_load_si256((__m256i const*)p_pDst));
	if (_mm256_testz_si256(xor, xor)) {
		return 0;
	}
	xor = _mm256_xor_si256(_mm256_load_si256((__m256i const*)(p_pSrc + 32)),
			               _mm256_load_si256((__m256i const*)(p_pDst + 32)));
	return !_mm256_testz_si256(xor, xor);
}

#endif // __AVX2__

inline int
str512Match(char const* p_pSrc,
		    char const* p_pDst)
{
#if defined(__AVX512F__)
	return str512Match_avx512(p_pSrc, p_pDst);
#elif defined(__AVX2__)
	return str512Match_avx2(p_pStr, p_pDst);
#else
	for (uint32_t i = 0u; i < 64u; ++i) {
		if (p_pSrc[i] != p_pDst[i]) {
			return 0;
		}
	}
	return 1;
#endif
}

inline int
str512SubMatch(char const*    p_pPat,
		       char const*    p_pStr,
			   const uint32_t p_patLen)
{
	char cat[128];
	strncpy(cat, p_pPat, p_patLen);
	cat[p_patLen] = '#';
	strcpy(&cat[p_patLen + 1], p_pStr);

	uint32_t pf[128], l = UINT32_MAX;
	pf[0] = UINT32_MAX;
	for (uint32_t i = 1u; i < 128u; ++i) {
		while (cat[i] != cat[l + 1]) {
			if (l == UINT32_MAX) {
				pf[i] = UINT32_MAX;
				goto NextChar;
			}
			l = pf[l];
		}
		pf[i] = ++l;
		if (l == p_patLen - 1) {
			return i - (p_patLen << 1);
		}
	NextChar:
		;
	}
	return 0;
}

