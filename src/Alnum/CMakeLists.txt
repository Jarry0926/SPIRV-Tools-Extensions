cmake_minimum_required(VERSION 3.25.2)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(spirv-alnum
	StrHash.c
	Alnum.c
)

target_compile_options(spirv-alnum
PUBLIC
    $<$<CONFIG:Debug>: -O0 -g>
    $<$<CONFIG:Release>: -O3 -flto -march=native -Rpass-missed=inline -Rpass-analysis=inline>
)

