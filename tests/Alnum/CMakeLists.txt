add_executable(test_alnum)

target_sources(test_alnum
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/StrMap.cpp
)
target_link_libraries(test_alnum
PRIVATE
  spirv_alnum
  GTest::gtest_main
)

gtest_discover_tests(test_alnum)

