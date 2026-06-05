add_library(spirv_alnum SHARED)

target_include_directories(spirv_alnum
PRIVATE
	"${SPIRV_TOOLS_EXT_DIR}"
)
target_sources(spirv_alnum
PRIVATE
#${CMAKE_CURRENT_SOURCE_DIR}/Alnum.c
  ${CMAKE_CURRENT_SOURCE_DIR}/StrMap.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Check.c
  $<TARGET_OBJECTS:utils>
)

