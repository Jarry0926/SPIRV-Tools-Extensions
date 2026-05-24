cmake_minimum_required(VERSION 4.3.1)

project(SPIRV-Tools-Plugin)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_executable(spirv-lower
    src/Substitude.c
    Main.c
)

target_compile_definitions(spirv-lower
PUBLIC
    $<$<CONFIG:Debug>:
        JK_DEBUG
    >
    $<$<CONFIG:Release>:
    >
)

target_compile_options(spirv-lower
PUBLIC
    #-ferror-limit=1
    $<$<CONFIG:Debug>:
        -O0
        -g
    >
    $<$<CONFIG:Release>:
        -O3
        -flto
        -march=native
        -Rpass-missed=inline
        -Rpass-analysis=inline
    >
)
