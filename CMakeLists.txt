cmake_minimum_required(VERSION 4.3)

project(spirv_tools_extensions)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SPIRV_TOOLS_EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${SPIRV_TOOLS_EXT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${SPIRV_TOOLS_EXT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${SPIRV_TOOLS_EXT_DIR}/lib")

include_directories("${SPIRV_TOOLS_EXT_DIR}/include")

add_compile_options(-ferror-limit=1)
add_compile_options($<$<CONFIG:Debug>:-g>)
add_compile_options($<$<CONFIG:Debug>:-O0>)
add_compile_options($<$<CONFIG:Release>:-O3>)
add_compile_options($<$<CONFIG:Release>:-flto>)
add_compile_options($<$<CONFIG:Release>:-march=native>)

add_link_options(-fuse-ld=lld)
add_link_options($<$<CONFIG:Release>:-flto>)

option(SPIRV_TOOLS_EXT_TOOLS "Build executable tools" OFF)
option(SPIRV_TOOLS_EXT_TESTS "Build tests" OFF)

add_subdirectory("${SPIRV_TOOLS_EXT_DIR}/utils")
add_subdirectory("${SPIRV_TOOLS_EXT_DIR}/src")

if (SPIRV_TOOLS_EXT_TOOLS)
  add_subdirectory("${SPIRV_TOOLS_EXT_DIR}/tools")
endif()

if (SPIRV_TOOLS_EXT_TESTS)
  add_subdirectory("${SPIRV_TOOLS_EXT_DIR}/tests")
endif()

