set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${SPIRV_TOOLS_EXT_DIR}")

find_package(GTest REQUIRED)

enable_testing()

include(GoogleTest)

add_subdirectory(utils)
add_subdirectory(Alnum)

