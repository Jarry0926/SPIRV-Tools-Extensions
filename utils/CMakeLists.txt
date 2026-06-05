add_library(utils OBJECT)
add_compile_options(-Wall -Werror)

target_sources(utils
PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Str.c"
)
target_compile_options(utils
PRIVATE
  $<$<CONFIG:Debug>: -g -O0>
  $<$<CONFIG:Release>: -O3 -flto>
)

