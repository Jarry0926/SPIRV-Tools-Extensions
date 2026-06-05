#pragma once

constexpr uint32_t STATUS_IN_FN_BIT           = 1u << 0;
constexpr uint32_t STATUS_HIT_ANNOTATION_BIT  = 1u << 1;
constexpr uint32_t STATUS_POST_ANNOTATION_BIT = 1u << 2;

int
checkFnScope(char const* const p_pStr,
		     uint32_t* const   p_pStatus);

int
checkAnnotation(char const* const p_pStr,
		        uint32_t* const   p_pStatus);

