#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

constexpr uint32_t ALNUM_OPT_DBG_INFO_BIT = 1u << 0;

char*
spvAlnum(const char* p_path);

#if defined(__cplusplus)
} // extern "C"
#endif

