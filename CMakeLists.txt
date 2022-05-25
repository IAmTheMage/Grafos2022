
cmake_minimum_required(VERSION 3.14)
project(GRAFOS)

set(INPUT_DIR "${PROJECT_SOURCE_DIR}/tests/input/")
set(EXPECTED_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/expected_output/")

configure_file(${PROJECT_SOURCE_DIR}/grafos/config.h.in ${PROJECT_SOURCE_DIR}/grafos/include/config.h) 
include_directories(${PROJECT_SOURCE_DIR}/grafos/include)


enable_testing()
add_subdirectory(grafos)
add_subdirectory(tests)

add_test(
  NAME  Test1
  COMMAND $<TARGET_FILE:test1>
  )
