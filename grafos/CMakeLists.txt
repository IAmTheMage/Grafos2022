file(GLOB source_files src/*.cpp include/*.h)
add_library(grafo ${source_files})
target_include_directories(grafo PUBLIC ${PROJECT_SOURCE_DIR}/include)