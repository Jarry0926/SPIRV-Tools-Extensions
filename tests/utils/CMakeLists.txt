add_executable(test_utils)

target_sources(test_utils
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Str.cpp
)
target_link_libraries(test_utils
PRIVATE
  $<TARGET_OBJECTS:utils>
  GTest::gtest_main
)

gtest_discover_tests(test_utils)

