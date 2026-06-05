cmake_minimum_required(VERSION 3.25.2)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(spirv-alnum)

target_sources(spirv-alnum
PRIVATE
  "${CMAKE_SOURCE_DIR}/Main.c"
  "${CMAKE_SOURCE_DIR}/ArgSet.c"
)
target_link_libraries(spirv-alnum
PRIVATE
  "${CMAKE_SOURCE_DIR}../../../lib/libspirv-alnum"
)
target_compile_options(spirv-alnum
PRIVATE
  $<$<CONFIG:Debug>: -O0 -g>
  $<$<CONFIG:Release>: -O3 -flto -march=native -Rpass-missed=inline -Rpass-analysis=inline>
)

