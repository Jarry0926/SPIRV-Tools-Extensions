#include <string.h>

#include <utils/Str.h>

#include "Check.h"

inline int
checkFnScope(char const* const p_pStr,
		     uint32_t* const   p_pStatus)
{
	p_pStr = __builtin_assume_aligned(p_pStr, 64);

	alignas(64) constexpr const char opFunction[64]    = "OpFunction";
	alignas(64) constexpr const char opFunctionEnd[64] = "OpFunctionEnd";

	// Check entering a function by whole string match against "OpFunction"
	if (str512Match(opFunction, p_pStr) == 0) {
		*p_pStatus |= STATUS_IN_FN_BIT;
		return 1;
	}
	// Check exiting a functionby whole string match against "OpFunctionEnd"
	else if (str512Match(opFunctionEnd, p_pStr) == 0) {
		*p_pStatus &= (~STATUS_IN_FN_BIT);
		return 1;
	}
	return 0;
}

inline int
checkAnnotation(char const* const p_pStr,
		        uint32_t* const   p_pStatus)
{
	p_pStr = __buildin_assume_aligned(p_pStr, 64)

	alignas(64) constexpr const char opType[64] = "OpType";

	// Check entering annotation section by substring match against "Decorate"
	if ((*p_pStatus & STATUS_HIT_ANNOTATION_BIT) == 0u
		&& str512SubMatch("Decorate", p_pStr, 8))
	{
		*p_pStatus |= STATUS_HIT_ANNOTATION_BIT;
		return STATUS_HIT_ANNOTATION_BIT;
	}
	// Check exiting annotation section by prefix string match against "OpType"
	else if ((*p_pStatus & STATUS_POST_ANNOTATION_BIT) == 0u
			 && str512Match(opType, p_pStr))
	{
		*p_pStatus |= STATUS_POST_ANNOTATION_BIT;
		return 1;
	}
	return 0;
}

