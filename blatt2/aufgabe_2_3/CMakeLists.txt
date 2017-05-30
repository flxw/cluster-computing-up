cmake_minimum_required(VERSION 3.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(montecarlo_pi montecarlo_pi.c)
target_link_libraries(montecarlo_pi pthread m)

add_executable(affine_montecarlo_pi affine_montecarlo_pi.c)
target_link_libraries(affine_montecarlo_pi pthread m)
