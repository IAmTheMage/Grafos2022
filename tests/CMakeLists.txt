
add_definitions(-DTEST_CASE=1)

add_executable(test1 test1.cc)
target_link_libraries(test1 PUBLIC grafo)

add_executable(test2 test2.cc)
target_link_libraries(test2 PUBLIC grafo)

add_executable(test3 test3.cc)
target_link_libraries(test3 PUBLIC grafo)



add_executable(test5 test5.cc)
target_compile_definitions(test5 PUBLIC -DTEST_SUIT=Console)
target_link_libraries(test5 PUBLIC grafo)


add_executable(test4 test4.cc)
target_link_libraries(test4 PUBLIC grafo)
target_compile_definitions(test4 PUBLIC -DTEST_SUIT=Console -DOUTPUTMODE_FILESYSTEM=0)

#add_executable(test6 test6.cc)
#target_link_libraries(test5 PUBLIC grafo)
#target_compile_definitions(test4 PUBLIC grafo)

add_executable(test7 test7.cc)
target_link_libraries(test7 PUBLIC grafo)