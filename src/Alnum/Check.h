#pragma once

constexpr uint32_t STATUS_IN_FN_BIT           = 1u << 0;
constexpr uint32_t STATUS_HIT_ANNOTATION_BIT  = 1u << 1;
constexpr uint32_t STATUS_POST_ANNOTATION_BIT = 1u << 2;

/**
 * @brief Check entering or exiting a function scope and change `p_pStatus`
 *
 * @param p_pStr: current string being parsed, must be 64-byte aligned
 * @param p_pStatus: bitmask of status to be changed
 *
 * @return 1 if status changed,
 *         0 otherwise
 */
int
checkFnScope(char const* const p_pStr,
		     uint32_t* const   p_pStatus);

/**
 * @brief Check entering or exiting the annotation section and change `p_pStatus`
 *
 * @param p_pStr: current string being parsed, must be 64-byte aligned
 * @param p_pStatus: bitmask of status to be change
 *
 * @return `STATUS_HIT_ANNOTATION_BIT` if it just entered the annotion section,
 *         1 if it has left the annotation section,
 *         0 if no status changed
 */
int
checkAnnotation(char const* const p_pStr,
		        uint32_t* const   p_pStatus);

