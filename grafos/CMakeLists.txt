file(GLOB source_files src/*.cc include/*.h)
add_library(grafo ${source_files})
target_include_directories(grafo PUBLIC ${PROJECT_SOURCE_DIR}/include)