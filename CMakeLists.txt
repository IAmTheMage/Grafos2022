
cmake_minimum_required(VERSION 3.14)
project(GRAFOS)

set(INPUT_DIR "${PROJECT_SOURCE_DIR}/tests/input/")
set(ROOT_DIR "${PROJECT_SOURCE_DIR}/")
set(EXPECTED_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/expected_output/")
set(OUTPUT_FILES_DIRECTORY "${PROJECT_SOURCE_DIR}/output/")

configure_file(${PROJECT_SOURCE_DIR}/grafos/config.h.in ${PROJECT_SOURCE_DIR}/grafos/include/config.h) 
include_directories(${PROJECT_SOURCE_DIR}/grafos/include)


enable_testing()
add_subdirectory(grafos)
add_subdirectory(tests)

# add_test(
# NAME  CountNumberOfNodes
# COMMAND $<TARGET_FILE:test1> in.txt out.txt 1 1 0
# )

# add_test(
#   NAME  SearchByNode
#   COMMAND $<TARGET_FILE:test2> in.txt out.txt 1 1 0
#   )

# add_test(
#   NAME  CreateDirectedEdge
#   COMMAND $<TARGET_FILE:test3> in.txt out.txt 1 1 0
# )

add_test(
  NAME  DirectClosure
  COMMAND $<TARGET_FILE:test4> in.txt out.txt 1 1 0
)

add_test(
  NAME  IndirectDirectClosure
  COMMAND $<TARGET_FILE:test5> in.txt out.txt 1 1 0
)